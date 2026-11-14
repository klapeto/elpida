#!/usr/bin/env python3
#
#  Copyright (c) 2025  Ioannis Panagiotopoulos
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
"""
Benchmark code parity checks on the disassembly of benchmark executables.

Starting from the timed functions (every Task::DoRunImpl() by default), the tool follows direct calls, tail calls and
function addresses taken in the code (e.g. `lea sqrtf(%rip), %r14; call *%r14`) to collect all code the timed part can
reach. Indirect calls through vtables or function pointers loaded from memory cannot be followed; they are counted.

Commands:
  report BIN             Per task: functions and instructions reached, indirect calls, and parity violations
                         (cpuid/xgetbv, ID register reads, getauxval, outline atomics, CPU feature probes).
                         Exit code 1 if any violation is found.
  compare A B            Compare the normalized disassembly of two builds, task by task. Exit code 1 on differences.
  dump BIN DIR           Write the normalized disassembly of every reached function into DIR, for diff -r or meld.

Normalization removes what legitimately differs between two links of the same code: addresses, PC-relative
displacements, branch targets (rewritten as offsets in the function), references to unnamed constants, and the
low 12 bits after an aarch64 `adrp`. Calls keep the demangled target name, so a call that resolves to a different
function shows up as a difference.

The binaries need a symbol table (not stripped). Release builds of Elpida keep it on every platform.
Tools are taken from --toolchain, then $ELPIDA_TOOLCHAIN/bin, then PATH.
"""

import argparse
import difflib
import hashlib
import os
import re
import shutil
import struct
import subprocess
import sys
from collections import defaultdict

DEFAULT_ROOTS = r"::DoRunImpl\(\)$"
# The StdLib benchmarks measure the platform's C library on purpose, so parity does not apply to them
DEFAULT_EXCLUDE = r"^Elpida::Benchmarks::StdLib::"

# Code that picks behavior per CPU at runtime, or reads the CPU's identity.
SUSPICIOUS_INSTRUCTIONS = re.compile(r"^(cpuid|xgetbv|rdrand|rdseed)\b|^mrs\s+\w+,\s*(ID_AA64\w*|MIDR_EL1|id_aa64\w*|midr_el1)")
SUSPICIOUS_CALLS = re.compile(
    r"^(__aarch64_(cas|swp|ldadd|ldclr|ldeor|ldset)\w*|__cpu_indicator_init|__cpu_model"
    r"|__cpu_features\w*|__init_cpu_features\w*|init_have_lse_atomics|IsProcessorFeaturePresent|__isa_available\w*"
    r"|android_getCpu\w*|__get_cpuid\w*|OPENSSL_cpuid_setup|OPENSSL_ia32cap\w*)$")

# Imported (shared library / DLL) functions that must not be reached from timed code: the computations musl-lite and
# mimalloc provide on every platform. Reaching one means the platform's libc does the work on that platform. OS services
# (memory mapping, threads, time, I/O) are listed with -v but are not violations.
PLATFORM_COMPUTE_IMPORTS = re.compile(
    r"^(__isoc\d+_\w+|mem\w+|__mem\w+_chk|str\w+|__str\w+_chk|wcs\w+|wmem\w+|bcmp|bcopy|bzero|ato[fil]\w*|qsort\w*"
    r"|bsearch|l?l?abs|l?l?div|rand\w*|srand\w*|is(alnum|alpha|blank|cntrl|digit|graph|lower|print|punct|space|upper|xdigit)\w*"
    r"|isw\w+|to(w?upper|w?lower)\w*|__ctype_\w+|_?mbr?\w+|wcr?tomb\w*|btowc|wctob|v?s?n?printf|v?s?scanf"
    r"|(a?(sin|cos|tan)h?|atan2|exp(2|10|m1)?|log(2|10|1p)?|pow|sqrt|cbrt|hypot|floor|ceil|l?l?round|l?l?rint|nearbyint"
    r"|trunc|fmod|remainder|frexp|ldexp|scalbn|modf|fabs|copysign|fmin|fmax|fma|erfc?|[lt]gamma)[fl]?"
    r"|malloc|calloc|realloc|free|posix_memalign|aligned_alloc|memalign|valloc|pvalloc|malloc_usable_size"
    r"|_aligned_\w+|_msize|_expand|_recalloc)$")
# On Windows mimalloc replaces the CRT allocator at runtime (detours), so calls to the CRT's malloc are expected
COFF_ALLOCATOR = re.compile(r"^(malloc|calloc|realloc|free|_aligned_\w+|_msize|_expand|_recalloc)$")

# Error and exception paths: they are listed but not followed, as they do not run in the timed loop and pull in large
# parts of the runtime (unwinder, abort messages, stdio)
DEFAULT_STOP = (r"^(__cxa_throw|__cxa_rethrow|__cxa_allocate_exception|__cxa_begin_catch|__cxa_end_catch|_Unwind_\w+"
                r"|__unw_\w+|libunwind::.*|__abort_message|abort|std::terminate\(\)|std::__terminate.*|__clang_call_terminate"
                r"|__assert\w*|_assert|_wassert|__fortify_fatal.*|__stack_chk_fail|std::__1::__throw_.*|std::__throw_.*"
                r"|ERR_\w+|ossl_err_\w+|__cxa_bad_\w+|__cxa_pure_virtual|__cxa_deleted_virtual|boost::throw_exception<.*|boost::re_detail_\d+::raise_.*)")

HEADER_RE = re.compile(r"^([0-9a-fA-F]+) <(.+)>:\s*$")
INSN_RE = re.compile(r"^\s*([0-9a-fA-F]+):\s+(.*)$")
# `0x401000 <foo>`, and riscv's `jalr 0x3d2(ra) <foo>`
REF_RE = re.compile(r"(?:0x)?[0-9a-fA-F]+(?:\(\w+\))? <([^<>]+?)(\+0x[0-9a-fA-F]+)?>")
COMMENT_RE = re.compile(r"\s+(#|//|@)\s.*$")
HEX_RE = re.compile(r"(?<![\w.])(-?)0x([0-9a-fA-F]+)\b")
RIP_RE = re.compile(r"-?0x[0-9a-fA-F]+\(%rip\)")
TLS_RE = re.compile(r"(%[fg]s):-0x[0-9a-fA-F]+")
ADRP_RE = re.compile(r"^adrp\s+(x\d+),")
ADR_RE = re.compile(r"^adr\s+(x\d+), (<[^<>]+>)$")
TP_MRS_RE = re.compile(r"^mrs\s+(x\d+), TPIDR_EL0", re.IGNORECASE)
TP_ADD_RE = re.compile(r"^add\s+(x\d+), (x\d+), #(?:0x[0-9a-fA-F]+|\d+)(, lsl #12)?$")
LO12_MEM_RE = re.compile(r"\[(x\d+)(?:, #(-?(?:0x[0-9a-fA-F]+|\d+)))?\]")
LO12_ADD_RE = re.compile(r"^(add\s+x\d+, )(x\d+)(, #)(0x[0-9a-fA-F]+|\d+)")
COMMENT_ADDR_RE = re.compile(r"(?:#|//)\s+0x([0-9a-fA-F]+)")
INDIRECT_CALL_RE = re.compile(r"^(call\w*\s+\*|jmp\w*\s+\*|blr\s|br\s|blx\s+r|bx\s+r(?!14|lr)|jalr\s|jr\s+(?!ra\b))")
ABS_INDIRECT_RE = re.compile(r"^(?:call|jmp)\w*\s+\*0x([0-9a-fA-F]+)$")  # i386 `jmpl *0x51e928`: an IAT entry
# lld's range extension thunks (`__ARMV7PILongThunk_foo` jumps to foo): their existence depends on the layout
THUNK_RE = re.compile(r"^__(?:ARM|Thumb|AArch64)\w*?Thunk_(.+)$")
# riscv64: `auipc rX, hi` and the instruction(s) that add the low part
AUIPC_RE = re.compile(r"^auipc\s+(\w+), (0x[0-9a-fA-F]+|\d+)$")
RV_LO_MEM_RE = re.compile(r"^(\w[\w.]*)\s+(?:(\w+), )?(-?0x[0-9a-fA-F]+|-?\d+)\((\w+)\)")
RV_LO_ADDI_RE = re.compile(r"^addi\s+(\w+), (\w+), (-?0x[0-9a-fA-F]+|-?\d+)$")
RV_NO_DEST = re.compile(r"^(s[bhwd]|fs[wdh]|b\w+|j|jr|ret|fence\w*|ecall|ebreak|nop|unimp)$")


def find_tool(name, toolchain):
    candidates = []
    if toolchain:
        candidates.append(os.path.join(toolchain, "bin", name))
    if os.environ.get("ELPIDA_TOOLCHAIN"):
        candidates.append(os.path.join(os.environ["ELPIDA_TOOLCHAIN"], "bin", name))
    for c in candidates:
        if os.access(c, os.X_OK):
            return c
    found = shutil.which(name)
    if not found:
        sys.exit(f"error: {name} not found (use --toolchain or set ELPIDA_TOOLCHAIN)")
    return found


def run(cmd, stdin=None):
    result = subprocess.run(cmd, input=stdin, capture_output=True, text=True)
    if result.returncode != 0 and not result.stdout:
        sys.exit(f"error: {' '.join(cmd[:3])}...: {result.stderr.strip()}")
    return result.stdout


class Binary:
    def __init__(self, path, tools):
        self.path = path
        self.tools = tools
        self.format = ""
        self.sections = []
        self._read_headers()
        self.functions = set()
        self.symbols = set()
        self.function_at = {}
        self.address_of = {}
        self.whole = None  # address -> instructions of the whole image (ARM)
        self.iat = {}  # IAT entry address -> imported symbol (COFF)
        self.imp_symbols = {}  # __imp_ symbol -> address (COFF): an IAT entry, or a pointer the image defines itself
        self.local_pointers = {}  # address of an __imp_ pointer defined in the image -> the function it points to
        self.coff_underscore = self.format == "coff-i386"
        self._read_symbols()
        self.thunks = {}  # lld thunk symbol (or locally defined __imp_ pointer) -> the function it jumps to
        for name in self.functions:
            m = THUNK_RE.match(name)
            if m and m.group(1) in self.functions:
                self.thunks[name] = m.group(1)
        self.got = {}  # GOT slot address -> imported symbol (ELF)
        self.plt = {}  # PLT entry address -> imported symbol, where objdump doesn't name them (riscv64)
        self.relative = {}  # address -> value set by a RELATIVE relocation (ELF)
        self.irelative = 0
        self._read_imports()
        self.loaded = []  # (address, file offset, size) of the sections in the file (ELF)
        self.pointer_size = 8 if "64" in self.format else 4
        self._read_elf_layout()
        self._read_pe_imports()
        if "riscv" in self.format:
            self._read_riscv_plt()
        self.display = self._demangle(sorted(self.functions))
        self.listing = {}  # symbol -> list of (mnemonic+operands raw, refs)
        self.edges = defaultdict(set)
        self.indirect = defaultdict(int)
        self.flags = defaultdict(list)
        self.flag_calls = defaultdict(list)  # (text, target) of calls to functions with suspicious names
        self.imported = defaultdict(set)
        self.stopped = set()

    def _read_headers(self):
        out = run([self.tools["objdump"], "-h", self.path])
        for line in out.splitlines():
            m = re.search(r"file format (\S+)", line)
            if m:
                self.format = m.group(1)
            parts = line.split()
            # Idx Name Size VMA Type
            if len(parts) >= 4 and parts[0].isdigit():
                try:
                    size, vma = int(parts[2], 16), int(parts[3], 16)
                except ValueError:
                    continue
                # Debug sections have VMA 0; PIE images start near 0, so ignore the first 64 KiB, where
                # small immediates and stack offsets live
                if size and vma and len(parts) >= 5 and parts[4] != "DEBUG":
                    self.sections.append((max(vma, 0x10000), vma + size))

    def _read_symbols(self):
        out = run([self.tools["nm"], "--defined-only", self.path])
        for line in out.splitlines():
            parts = line.split(None, 2)
            if len(parts) != 3:
                continue
            address, kind, name = parts
            self.symbols.add(name)
            if name.startswith("__imp_"):
                self.imp_symbols[name] = int(address, 16)
            if kind in "TtWw":
                self.functions.add(name)
                self.function_at.setdefault(int(address, 16), name)
                self.address_of[name] = int(address, 16)
        if not self.functions:
            sys.exit(f"error: {self.path} has no symbols (stripped?)")

    def _read_imports(self):
        if not self.format.startswith("elf"):
            return
        out = run([self.tools["readelf"], "-r", "--wide", self.path])
        for line in out.splitlines():
            parts = line.split()
            if len(parts) < 3 or not re.match(r"^[0-9a-fA-F]+$", parts[0]):
                continue
            if "IRELATIV" in parts[2]:
                self.irelative += 1
            elif "RELATIVE" in parts[2] and len(parts) == 4:
                self.relative[int(parts[0], 16)] = int(parts[3], 16)
            elif ("GLOB_DAT" in parts[2] or "JUMP_SLOT" in parts[2]) and len(parts) >= 5 and int(parts[3], 16) == 0:
                self.got[int(parts[0], 16)] = parts[4].split("@")[0]

    def _read_elf_layout(self):
        if not self.format.startswith("elf"):
            return
        with open(self.path, "rb") as f:
            header = f.read(64)
            is64 = header[4] == 2
            if is64:
                shoff, = struct.unpack_from("<Q", header, 0x28)
                shentsize, shnum = struct.unpack_from("<HH", header, 0x3A)
            else:
                shoff, = struct.unpack_from("<I", header, 0x20)
                shentsize, shnum = struct.unpack_from("<HH", header, 0x2E)
            f.seek(shoff)
            table = f.read(shentsize * shnum)
        for i in range(shnum):
            if is64:
                _, sh_type, flags, addr, offset, size = struct.unpack_from("<IIQQQQ", table, i * shentsize)
            else:
                _, sh_type, flags, addr, offset, size = struct.unpack_from("<IIIIII", table, i * shentsize)
            if flags & 2 and sh_type != 8:  # SHF_ALLOC, not SHT_NOBITS
                self.loaded.append((addr, offset, size))

    def _read_pe_imports(self):
        """Sort the __imp_ symbols into IAT entries (imports) and pointers the image defines itself (e.g. musl-lite's
        __imp_isalpha, which replaces msvcrt's). lld merges .idata into .rdata, so the IAT data directory tells."""
        if not self.imp_symbols:
            return
        with open(self.path, "rb") as f:
            data = f.read(4096)
            pe, = struct.unpack_from("<I", data, 0x3C)
            sections, = struct.unpack_from("<H", data, pe + 6)
            optional_size, = struct.unpack_from("<H", data, pe + 20)
            optional = pe + 24
            magic, = struct.unpack_from("<H", data, optional)
            if magic == 0x20B:  # PE32+
                image_base, = struct.unpack_from("<Q", data, optional + 24)
                directories = optional + 112
            else:
                image_base, = struct.unpack_from("<I", data, optional + 28)
                directories = optional + 96
            iat_rva, iat_size = struct.unpack_from("<II", data, directories + 12 * 8)
            table = optional + optional_size
            for i in range(sections):
                _, virtual_size, rva, raw_size, raw_offset = struct.unpack_from("<8sIIII", data, table + i * 40)
                self.loaded.append((image_base + rva, raw_offset, min(virtual_size, raw_size)))
        iat = (image_base + iat_rva, image_base + iat_rva + iat_size)
        for name, address in self.imp_symbols.items():
            imported = name[6:]
            if self.coff_underscore and imported.startswith("_"):
                imported = imported[1:]
            if iat[0] <= address < iat[1]:
                self.iat[address] = imported
                continue
            target = self.function_at.get(self.pointer_at(address))
            if target:
                self.thunks[name] = target
                self.local_pointers[address] = target
            else:
                self.iat[address] = imported  # unknown: treat it as an import, as before

    def local_pointer_call(self, insn):
        """The function an indirect call through a locally defined __imp_ pointer reaches (i386 `calll *0x50e490`)"""
        m = ABS_INDIRECT_RE.match(re.sub(r"\s+", " ", COMMENT_RE.sub("", insn)).strip())
        return self.local_pointers.get(int(m.group(1), 16)) if m else None

    def pointer_at(self, address):
        """The pointer stored at an address of the image (e.g. a GOT slot), as the loader would see it"""
        if address in self.relative:
            return self.relative[address]
        for addr, offset, size in self.loaded:
            if addr <= address < addr + size:
                with open(self.path, "rb") as f:
                    f.seek(offset + address - addr)
                    data = f.read(self.pointer_size)
                return int.from_bytes(data, "little") if len(data) == self.pointer_size else None
        return None

    def _read_riscv_plt(self):
        """objdump doesn't name riscv64 PLT entries (`jalr 0x644(ra) <write+0x39d9f0>`). Each entry is
        `auipc t3, hi; ld t3, lo(t3); jalr t1, t3`: the GOT slot it loads names the import."""
        out = run([self.tools["objdump"], "-d", "--no-show-raw-insn", "-j", ".plt", self.path])
        lines = [(int(m.group(1), 16), m.group(2).strip()) for m in map(INSN_RE.match, out.splitlines()) if m]
        for (address, insn), (_, following) in zip(lines, lines[1:]):
            m = AUIPC_RE.match(re.sub(r"\s+", " ", insn))
            mm = RV_LO_MEM_RE.match(re.sub(r"\s+", " ", following))
            if m and mm and mm.group(1) == "ld" and mm.group(4) == m.group(1):
                slot = address + self._auipc_offset(m.group(2)) + int(mm.group(3), 0)
                if slot in self.got:
                    self.plt[address] = self.got[slot]

    @staticmethod
    def _auipc_offset(immediate):
        value = int(immediate, 0) & 0xFFFFF
        return (value - (1 << 20) if value & 0x80000 else value) << 12

    def _annotate_riscv(self, lines):
        """Resolve auipc pairs (function addresses, GOT loads, calls into the PLT), which objdump leaves unresolved
        or names after the nearest symbol, by appending a `# 0xADDR <symbol>` comment"""
        his = {}
        result = []
        for address, line in lines:
            text = re.sub(r"\s+", " ", COMMENT_RE.sub("", line)).strip()
            note = ""
            m = AUIPC_RE.match(text)
            if m:
                his[m.group(1)] = address + self._auipc_offset(m.group(2))
                result.append(line)
                continue
            target = None
            mm = RV_LO_MEM_RE.match(text)
            ma = RV_LO_ADDI_RE.match(text)
            if mm and mm.group(4) in his:
                target = his[mm.group(4)] + int(mm.group(3), 0)
                if mm.group(1) == "ld":  # a GOT load
                    if target in self.got:
                        note = f" # 0x{target:x}"
                    else:
                        value = self.pointer_at(target)
                        if value in self.function_at:
                            note = f" # 0x{value:x} <{self.function_at[value]}>"
                    target = None
            elif ma and ma.group(2) in his:
                target = his[ma.group(2)] + int(ma.group(3), 0)
            mj = re.match(r"^(?:jal|j) (?:\w+, )?0x([0-9a-fA-F]+)\b", text)
            if mj:  # a call relaxed by the linker to `jal`, straight into the PLT
                target = int(mj.group(1), 16)
            if target is not None:
                if target in self.plt:
                    note = f" # 0x{target:x}"
                elif target in self.function_at:
                    note = f" # 0x{target:x} <{self.function_at[target]}>"
            # The destination register no longer holds the high part
            md = re.match(r"^([\w.]+) (\w+)", text)
            if md and not RV_NO_DEST.match(md.group(1)):
                his.pop(md.group(2), None)
            if md and md.group(1) in ("jal", "jalr", "call"):
                his.pop("ra", None)
            if note:
                line = REF_RE.sub("", COMMENT_RE.sub("", line)).rstrip() + note
            result.append(line)
        return result

    def _annotate_aarch64(self, lines):
        """Resolve adrp+ldr (GOT loads, e.g. calls with -fno-plt) and adrp+add (addresses of functions), which objdump
        leaves unresolved, by appending a `// 0xADDR <symbol>` comment like the one objdump prints on x86"""
        pages = {}
        result = []
        for line in lines:
            note = ""
            m = re.match(r"^adrp\s+(x\d+), 0x([0-9a-fA-F]+)", line)
            if m:
                pages[m.group(1)] = int(m.group(2), 16)
                result.append(line)
                continue
            m = re.match(r"^ldr\s+(x\d+), \[(x\d+), #(0x[0-9a-fA-F]+|\d+)\]$", line)
            if m and m.group(2) in pages:
                slot = pages[m.group(2)] + int(m.group(3), 0)
                if slot in self.got:
                    note = f" // 0x{slot:x}"
                else:
                    target = self.pointer_at(slot)
                    if target in self.function_at:
                        note = f" // 0x{target:x} <{self.function_at[target]}>"
            m2 = re.match(r"^add\s+(x\d+), (x\d+), #(0x[0-9a-fA-F]+|\d+)$", line)
            if m2 and m2.group(2) in pages:
                target = pages[m2.group(2)] + int(m2.group(3), 0)
                if target in self.function_at:
                    note = f" // 0x{target:x} <{self.function_at[target]}>"
            # The destination register no longer holds the page
            m3 = re.match(r"^(\w+)\s+(x\d+),", line)
            if m3 and m3.group(1) not in ("str", "stp", "stur", "strb", "strh", "cmp", "cmn", "tst", "cbz", "cbnz",
                                          "tbz", "tbnz", "prfm"):
                pages.pop(m3.group(2), None)
            result.append(line + note)
        return result

    def import_of(self, insn):
        """The imported (shared library / DLL) function an instruction refers to, if any"""
        for ref in REF_RE.finditer(insn):
            target = ref.group(1)
            if target.endswith("@plt"):
                return target[:-4]
            if target.startswith("__imp_") and target not in self.thunks:
                name = target[6:]
                return name[1:] if self.coff_underscore and name.startswith("_") else name
        m = COMMENT_ADDR_RE.search(insn)
        if m:
            address = int(m.group(1), 16)
            if address in self.got:
                return self.got[address]
            if address in self.plt:
                return self.plt[address]
        m = ABS_INDIRECT_RE.match(re.sub(r"\s+", " ", insn).strip())
        if m and int(m.group(1), 16) in self.iat:
            return self.iat[int(m.group(1), 16)]
        return None

    def _demangle(self, names):
        stripped = [n[1:] if self.coff_underscore and n.startswith("_") else n for n in names]
        out = run([self.tools["cxxfilt"]], stdin="\n".join(stripped) + "\n").splitlines()
        if len(out) != len(names):
            return {n: s for n, s in zip(names, stripped)}
        return dict(zip(names, out))

    def name(self, symbol):
        return self.display.get(symbol, symbol)

    def in_image(self, value):
        return any(lo <= value < hi for lo, hi in self.sections)

    def _disassemble_whole(self):
        """On ARM, objdump's --disassemble-symbols stops at the first literal pool ($d mapping symbol) in a function,
        so the whole image is disassembled once and split at the function symbols"""
        self.whole = defaultdict(list)
        current = None
        # Streamed: the listing of a large image (the Compilation benchmark) is several GB as one string
        with subprocess.Popen([self.tools["objdump"], "-d", "--no-show-raw-insn", self.path], stdout=subprocess.PIPE,
                              stderr=subprocess.DEVNULL, text=True) as process:
            for line in process.stdout:
                m = HEADER_RE.match(line)
                if m:
                    current = int(m.group(1), 16)
                    continue
                m = INSN_RE.match(line)
                if m and current is not None:
                    self.whole[current].append(m.group(2).strip())
        if process.returncode != 0 and not self.whole:
            sys.exit(f"error: {self.tools['objdump']} -d {self.path} failed")

    def disassemble(self, symbols):
        symbols = [s for s in symbols if s not in self.listing]
        if self.format == "elf32-littlearm":
            if self.whole is None:
                self._disassemble_whole()
            for s in symbols:
                self.listing[s] = self.whole.get(self.address_of.get(s, -1) & ~1, [])  # Thumb symbols have bit 0 set
            return
        for i in range(0, len(symbols), 400):
            chunk = symbols[i:i + 400]
            out = run([self.tools["objdump"], "-d", "--no-show-raw-insn", "--disassemble-symbols=" + ",".join(chunk),
                       self.path])
            current = None
            raw = defaultdict(list)
            for line in out.splitlines():
                m = HEADER_RE.match(line)
                if m:
                    current = m.group(2)
                    continue
                m = INSN_RE.match(line)
                if m and current is not None:
                    raw[current].append((int(m.group(1), 16), m.group(2).strip()))
            for s in chunk:
                lines = raw.get(s, [])
                if "aarch64" in self.format:
                    self.listing[s] = self._annotate_aarch64([text for _, text in lines])
                elif "riscv" in self.format:
                    self.listing[s] = self._annotate_riscv(lines)
                else:
                    self.listing[s] = [text for _, text in lines]

    def explore(self, roots, max_functions, stop):
        pending = list(roots)
        seen = set(pending)
        truncated = False
        while pending:
            self.disassemble(pending)
            following = []
            for symbol in pending:
                for insn in self.listing[symbol]:
                    text = COMMENT_RE.sub("", insn)
                    if INDIRECT_CALL_RE.match(text) and not REF_RE.search(insn):
                        self.indirect[symbol] += 1
                    if SUSPICIOUS_INSTRUCTIONS.match(text):
                        self.flags[symbol].append(text)
                    imported = self.import_of(insn)
                    if imported:
                        self.imported[symbol].add(imported)
                        if SUSPICIOUS_CALLS.match(imported):
                            self.flags[symbol].append(f"{text.split()[0]} {imported} (imported)")
                        continue
                    targets = [(m.group(1), m.group(2)) for m in REF_RE.finditer(insn)]
                    if self.local_pointer_call(insn):
                        targets.append((self.local_pointer_call(insn), None))
                    for target, offset in targets:
                        # symbol+offset is data, a jump inside a function or objdump's guess for an adrp page
                        if not offset:
                            target = self.thunks.get(target, target)
                        if offset or target == symbol or target not in self.functions:
                            continue
                        self.edges[symbol].add(target)
                        if SUSPICIOUS_CALLS.match(self.name(target)):
                            self.flag_calls[symbol].append((f"{text.split()[0]} {self.name(target)}", target))
                        if target not in seen and stop.match(self.name(target)):
                            seen.add(target)
                            self.stopped.add(target)
                            continue
                        if target not in seen:
                            if len(seen) >= max_functions:
                                truncated = True
                                continue
                            seen.add(target)
                            following.append(target)
            pending = following
        return truncated

    def empty(self, symbol):
        """A function that only returns (e.g. OpenSSL's OPENSSL_cpuid_setup with no-asm)"""
        body = [re.sub(r"\s+", " ", COMMENT_RE.sub("", i)).strip() for i in self.listing.get(symbol, [])]
        while body and re.match(r"^(nop\w*|int3|udf|\.word|<unknown>)\b", body[-1]):
            body.pop()
        return len(body) == 1 and re.match(r"^(ret\w*|bx lr)$", body[0]) is not None

    def violations(self, symbol):
        return self.flags[symbol] + [text for text, target in self.flag_calls[symbol] if not self.empty(target)]

    def closure(self, root):
        result, stack = {root}, [root]
        while stack:
            for t in self.edges[stack.pop()]:
                if t not in result:
                    result.add(t)
                    stack.append(t)
        return result

    def normalized(self, symbol, mnemonics_only=False):
        lines = []
        adrp_regs = set()
        tp_regs = set()
        auipc_regs = set()
        for insn in self.listing.get(symbol, []):
            imported = self.import_of(insn)
            refs = {m.group(0): m for m in REF_RE.finditer(insn)}
            text = COMMENT_RE.sub("", insn)
            if imported:
                # PLT stub, GOT slot or IAT entry: the same on both sides only if the import is
                refs = {}
                text = REF_RE.sub("", text).strip() + f" ; <import {imported}>"
            elif self.local_pointer_call(insn):
                # i386 call through an __imp_ pointer the image defines: the address differs between builds
                text = re.sub(r"0x[0-9a-fA-F]+", "", text).strip() + f" ; <{self.name(self.local_pointer_call(insn))}>"
            # A symbol named in the comment (e.g. the target of a RIP-relative lea) is kept as an annotation
            comment_refs = [m for k, m in refs.items() if k not in text]

            def ref_text(m):
                target, offset = m.group(1), m.group(2) or ""
                if not offset:
                    target = self.thunks.get(target, target)
                if target == symbol:
                    return f"<.{offset}>"
                if target in self.symbols and not offset:
                    return f"<{self.name(target)}>"
                if target in self.functions:
                    return f"<{self.name(target)}{offset}>"
                return "<?>"

            text = REF_RE.sub(ref_text, text)
            text = RIP_RE.sub("X(%rip)", text)
            # x86 static TLS (local/initial exec): negative offsets from the thread pointer are the variables' places in
            # the TLS block, chosen at link time. Positive ones (stack guard %fs:0x28, TCB/TEB fields) are ABI and kept.
            text = TLS_RE.sub(r"\1:-TLS", text)
            # aarch64 static TLS (local exec): `mrs xN, TPIDR_EL0` then `add xN, xN, #hi, lsl #12` / `add xN, xN, #lo`.
            # The offsets are link layout (bionic reserves a larger area before the TLS block than glibc).
            m = TP_MRS_RE.match(text)
            if m:
                tp_regs.add(m.group(1))
            elif tp_regs:
                mm = TP_ADD_RE.match(re.sub(r"\s+", " ", text).strip())
                if mm and mm.group(2) in tp_regs:
                    tp_regs.add(mm.group(1))
                    text = f"add {mm.group(1)}, {mm.group(2)}, #TLS{mm.group(3) or ''}"
            m = ADRP_RE.match(text)
            if m:
                adrp_regs.add(m.group(1))
            elif adrp_regs:
                text = LO12_MEM_RE.sub(lambda mm: f"[{mm.group(1)}, #lo12]" if mm.group(1) in adrp_regs else mm.group(0),
                                       text)
                text = LO12_ADD_RE.sub(lambda mm: f"{mm.group(1)}{mm.group(2)}{mm.group(3)}lo12"
                                       if mm.group(2) in adrp_regs else mm.group(0), text)
            if "riscv" in self.format:
                # auipc+lo pairs are PC-relative: both immediates are layout
                flat = re.sub(r"\s+", " ", text).strip()
                m = AUIPC_RE.match(flat)
                if m:
                    auipc_regs.add(m.group(1))
                    text = f"auipc {m.group(1)}, <hi>"
                elif auipc_regs:
                    mm = RV_LO_MEM_RE.match(flat)
                    ma = RV_LO_ADDI_RE.match(flat)
                    if mm and mm.group(4) in auipc_regs:
                        text = flat[:mm.start(3)] + "lo" + flat[mm.end(3):]
                    elif ma and ma.group(2) in auipc_regs:
                        text = f"addi {ma.group(1)}, {ma.group(2)}, lo"
                    md = re.match(r"^([\w.]+) (\w+)", flat)
                    if md and not RV_NO_DEST.match(md.group(1)):
                        auipc_regs.discard(md.group(2))
                    if md and md.group(1) in ("jal", "jalr", "call"):
                        auipc_regs.discard("ra")
            # ARM literal pools inside functions: PC-relative offsets and addresses, i.e. layout
            # (objdump prints data with its raw bytes even with --no-show-raw-insn)
            text = re.sub(r"^(?:[0-9a-fA-F]{2} )*\s*\.(word|short|byte)\s+\S+", r".\1 <literal>", text)
            if "64" not in self.format:  # 64-bit code is PIC: addresses only appear as the references handled above
                text = HEX_RE.sub(lambda mm: "ADDR" if self.in_image(int(mm.group(2), 16)) else mm.group(0), text)
            text = re.sub(r"\s+", " ", text).strip()
            if comment_refs:
                text += " ; " + " ".join(ref_text(m) for m in comment_refs)
            m = ADRP_RE.match(text)
            if m:
                # The page of an adrp is layout; objdump names it after whatever symbol happens to start that page,
                # in the operand or in the comment
                text = f"adrp {m.group(1)}, <page>"
            m = ADR_RE.match(text)
            if m and lines and lines[-1] == "nop":
                # lld relaxes adrp+add to nop+adr when the target is within 1 MB, which depends on the layout:
                # write it back as the adrp+add pair
                lines[-1] = f"adrp {m.group(1)}, <page>"
                text = f"add {m.group(1)}, {m.group(1)}, #lo12 ; {m.group(2)}"
            if mnemonics_only:
                text = text.split(" ", 1)[0]
            lines.append(text)
        # Trailing padding differs with alignment of whatever follows
        while lines and re.match(r"^(nop\w*|int3|udf|\.word|\.long|\.short|\.byte|<unknown>)\b", lines[-1]):
            lines.pop()
        return lines


def load(path, args):
    tools = {k: find_tool(v, args.toolchain) for k, v in
             (("objdump", "llvm-objdump"), ("nm", "llvm-nm"), ("cxxfilt", "llvm-cxxfilt"),
              ("readelf", "llvm-readelf"))}
    binary = Binary(path, tools)
    pattern = re.compile(args.roots)
    exclude = re.compile(args.exclude) if args.exclude else None
    roots = {}
    excluded = 0
    for symbol in binary.functions:
        if pattern.search(binary.name(symbol)):
            if exclude and exclude.search(binary.name(symbol)):
                excluded += 1
                continue
            roots[binary.name(symbol)] = symbol
    # Lambdas and local functions of a root (e.g. the Exec() lambda) belong to it, even if only reached indirectly
    by_prefix = defaultdict(list)
    for symbol in binary.functions:
        name = binary.name(symbol)
        cut = name.find("::$")
        if cut < 0:
            cut = name.find("::{lambda")
        if cut > 0 and name[:cut] in roots:
            by_prefix[name[:cut]].append(symbol)
    for root_name, locals_ in by_prefix.items():
        for s in locals_:
            binary.edges[roots[root_name]].add(s)
    if not roots:
        if excluded:
            print(f"{path}: all {excluded} tasks are excluded ({args.exclude}); the StdLib benchmarks measure the "
                  f"platform's C library and are not held to parity. Use --exclude '' to check them anyway.")
            sys.exit(0)
        sys.exit(f"error: no function in {path} matches {args.roots}")
    all_start = list(roots.values()) + [s for v in by_prefix.values() for s in v]
    truncated = binary.explore(all_start, args.max_functions, re.compile(args.stop))
    if truncated:
        print(f"warning: {path}: stopped after {args.max_functions} functions (--max-functions); "
              f"the reachable code is incomplete", file=sys.stderr)
    return binary, roots


def by_name(binary, symbols):
    return {binary.name(s): s for s in symbols}


def cmd_report(args):
    binary, roots = load(args.binary, args)
    print(f"{args.binary} ({binary.format})")
    violations = 0
    for root_name in sorted(roots):
        reach = binary.closure(roots[root_name])
        stopped = len(reach & binary.stopped)
        reach = reach - binary.stopped
        insns = sum(len(binary.listing.get(s, [])) for s in reach)
        indirect = sum(binary.indirect[s] for s in reach)
        flagged = sorted({(binary.name(s), f) for s in reach for f in binary.violations(s)})
        imports = defaultdict(set)
        for s in reach:
            for i in binary.imported[s]:
                imports[i].add(binary.name(s))
        bad_imports = sorted(i for i in imports if PLATFORM_COMPUTE_IMPORTS.match(i)
                             and not (binary.format.startswith("coff") and COFF_ALLOCATOR.match(i)))
        violations += len(flagged) + len(bad_imports)
        print(f"  {root_name}")
        print(f"      {len(reach)} functions, {insns} instructions, {indirect} indirect calls, "
              f"{len(imports)} imported functions, {stopped} error paths not followed")
        for i in bad_imports:
            print(f"      VIOLATION: calls the system library's {i} (from {', '.join(sorted(imports[i])[:3])})")
        if args.verbose:
            for i in sorted(set(imports) - set(bad_imports)):
                print(f"        import: {i}")
        if args.verbose:
            for s in sorted(reach, key=binary.name):
                print(f"        {binary.name(s)} ({len(binary.listing.get(s, []))})")
        for s, f in flagged:
            print(f"      VIOLATION in {s}: {f}")
    if binary.irelative:
        print(f"  VIOLATION: {binary.irelative} IRELATIVE relocations (IFUNCs, runtime CPU dispatch) in the binary")
        violations += 1
    print(f"{len(roots)} tasks, {violations} violations")
    return 1 if violations else 0


def cmd_compare(args):
    a, roots_a = load(args.a, args)
    b, roots_b = load(args.b, args)
    print(f"A: {args.a} ({a.format})")
    print(f"B: {args.b} ({b.format})")
    if a.format != b.format:
        print("note: different object formats (ABI); register allocation and calling sequences will differ. "
              "Consider --mnemonics.")
    different = 0
    for root_name in sorted(set(roots_a) | set(roots_b)):
        if root_name not in roots_a or root_name not in roots_b:
            print(f"ONLY IN {'A' if root_name in roots_a else 'B'}  {root_name}")
            different += 1
            continue
        funcs_a = by_name(a, a.closure(roots_a[root_name]) - a.stopped)
        funcs_b = by_name(b, b.closure(roots_b[root_name]) - b.stopped)
        only_a = sorted(set(funcs_a) - set(funcs_b))
        only_b = sorted(set(funcs_b) - set(funcs_a))
        changed = []
        for name in sorted(set(funcs_a) & set(funcs_b)):
            la = a.normalized(funcs_a[name], args.mnemonics)
            lb = b.normalized(funcs_b[name], args.mnemonics)
            if la != lb:
                changed.append((name, la, lb))
        same = len(set(funcs_a) & set(funcs_b)) - len(changed)
        if not only_a and not only_b and not changed:
            print(f"SAME       {root_name}  ({same} functions)")
            continue
        different += 1
        print(f"DIFFERENT  {root_name}  ({same} same, {len(changed)} changed, {len(only_a)} only in A, "
              f"{len(only_b)} only in B)")
        for name in only_a:
            print(f"    only in A: {name} ({len(a.listing.get(funcs_a[name], []))} instructions)")
        for name in only_b:
            print(f"    only in B: {name} ({len(b.listing.get(funcs_b[name], []))} instructions)")
        for name, la, lb in changed:
            print(f"    changed:   {name} ({len(la)} vs {len(lb)} instructions)")
            if args.show_diff:
                diff = list(difflib.unified_diff(la, lb, "A", "B", lineterm="", n=2))
                for line in diff[2:2 + args.show_diff]:
                    print(f"        {line}")
                if len(diff) > 2 + args.show_diff:
                    print(f"        ... {len(diff) - 2 - args.show_diff} more lines")
    print(f"{different} of {len(set(roots_a) | set(roots_b))} tasks differ")
    return 1 if different else 0


def safe_name(name):
    clean = re.sub(r"[^A-Za-z0-9_.-]+", "_", name).strip("_")
    if len(clean) > 100:
        clean = clean[:100] + "-" + hashlib.sha1(name.encode()).hexdigest()[:8]
    return clean


def cmd_dump(args):
    binary, roots = load(args.binary, args)
    os.makedirs(args.dir, exist_ok=True)
    written = set()
    with open(os.path.join(args.dir, "tasks.txt"), "w") as index:
        for root_name in sorted(roots):
            closure = binary.closure(roots[root_name]) - binary.stopped
            reach = sorted(binary.name(s) for s in closure)
            index.write(root_name + "\n" + "".join(f"    {n}\n" for n in reach))
            for s in closure:
                if s in written:
                    continue
                written.add(s)
                with open(os.path.join(args.dir, safe_name(binary.name(s)) + ".s"), "w") as f:
                    f.write(f"; {binary.name(s)}\n")
                    f.write("\n".join(binary.normalized(s, args.mnemonics)) + "\n")
    print(f"{len(written)} functions of {len(roots)} tasks written to {args.dir}")
    return 0


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--toolchain", help="llvm-mingw toolchain directory (default: $ELPIDA_TOOLCHAIN, then PATH)")
    parser.add_argument("--roots", default=DEFAULT_ROOTS,
                        help=f"regex on demangled names of the timed functions (default: {DEFAULT_ROOTS})")
    parser.add_argument("--exclude", default=DEFAULT_EXCLUDE,
                        help=f"regex on demangled names of timed functions to skip (default: {DEFAULT_EXCLUDE}; "
                             f"'' for none)")
    parser.add_argument("--stop", default=DEFAULT_STOP,
                        help="regex on demangled names of functions not to follow (default: error and exception paths)")
    parser.add_argument("--max-functions", type=int, default=5000,
                        help="stop following calls after this many functions (default: 5000)")
    parser.add_argument("--mnemonics", action="store_true",
                        help="compare/dump only instruction mnemonics (ignores registers; for different ABIs)")
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("report", help="list reachable code and parity violations per task")
    p.add_argument("binary")
    p.add_argument("-v", "--verbose", action="store_true", help="list every reached function")
    p.set_defaults(func=cmd_report)
    p = sub.add_parser("compare", help="compare two builds task by task")
    p.add_argument("a")
    p.add_argument("b")
    p.add_argument("--show-diff", type=int, default=0, metavar="N", help="print up to N diff lines per function")
    p.set_defaults(func=cmd_compare)
    p = sub.add_parser("dump", help="write normalized disassembly per function")
    p.add_argument("binary")
    p.add_argument("dir")
    p.set_defaults(func=cmd_dump)
    args = parser.parse_args()
    sys.exit(args.func(args))


if __name__ == "__main__":
    main()

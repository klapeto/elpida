# Virtual machine detection

Results from virtual machines are uploaded like any other, with a flag. The server keeps them but leaves them out of
the statistics. Decided 2026-11-14: flag rather than deny, because cloud instances (EC2, Azure, GCP) are always VMs
and their results are valid, and the server can change its policy without a client update.

Detection is best-effort. It keeps honest VM runs out of the statistics; it doesn't stop someone who hides the VM or
modifies the client.

## What is sent

`os.virtualization` in the info dumper's JSON and in the upload, next to `category`, `name` and `version`:

- `none`: bare metal, including a Windows host that runs on Hyper-V (see below), and containers (Docker is not a VM).
- Otherwise the VM: `kvm`, `qemu` (QEMU without KVM, or QEMU found through the firmware), `hyperv`, `wsl`, `vmware`,
  `virtualbox`, `xen`, `parallels`, `bhyve`, `acrn`, `bochs`, `apple`, `aws`, `gce`, `openstack`, `kubevirt`,
  `android-emulator`, or `other` for a hypervisor it doesn't know.
- `unknown`: only when a client parses info JSON without the field (`ModelBuilderJson`, the mobile app's model).

The Qt app shows it in the OS info box. Code: `Elpida::VirtualizationDetector`
(`include/Elpida/Platform/VirtualizationDetector.hpp`, `src/Elpida/Platform/VirtualizationDetector.*`), called by
`OsInfoLoader`.

## How

In order; the first match wins.

1. **Android:** the emulator properties `ro.kernel.qemu` / `ro.boot.qemu` = `1`, or `ro.hardware` = `ranchu` /
   `goldfish`. Checked first: the x86 emulator would otherwise report `kvm`.
2. **Linux:** WSL 2 from the kernel release (`/proc/sys/kernel/osrelease` contains `microsoft` or `WSL`).
3. **x86 (Linux, Android, Windows):** CPUID leaf 1 ECX bit 31 (hypervisor present), then the vendor at leaf
   `0x40000000`. Uses `__cpuid_count`, not `__get_cpuid_count`: the latter checks the leaf against the basic maximum
   leaf and never executes the hypervisor leaves.
   - `Microsoft Hv` with the `CreatePartitions` privilege (leaf `0x40000003` EBX bit 0) is the Hyper-V **root
     partition**: the host. Windows 10/11 with virtualization-based security (on by default on Windows 11) or the
     Hyper-V role runs its host OS on Hyper-V, so the hypervisor bit is set on bare metal. This check keeps those
     machines `none`; `systemd-detect-virt` does the same.
   - `Microsoft Hv` without it is a guest. KVM with Hyper-V enlightenments also reports `Microsoft Hv` and puts its
     own id at `0x40000100`, so that is checked for `KVMKVMKVM`.
4. **Linux:** `/sys/hypervisor/type` (Xen, including paravirtualized guests).
5. **Firmware:** the system manufacturer and product name, from DMI (`/sys/class/dmi/id/sys_vendor`,
   `product_name`) on Linux and from SMBIOS type 1 (`GetSystemFirmwareTable('RSMB')`) on Windows. Prefix matches of
   known VM strings. Microsoft's vendor string counts only with the product `Virtual Machine` (Surface devices have
   the same vendor), and `Amazon EC2` not with a `*.metal` product (bare-metal instances).
   `GetSystemFirmwareTable` is looked up at runtime: Windows XP x86 doesn't have it and relies on CPUID.
6. **Device tree** (ARM/RISC-V without DMI): `/proc/device-tree/hypervisor/compatible` (Xen) and QEMU's
   `linux,dummy-virt` machine.

## Tested (2026-11-14)

A static test program built from the detector sources, in throwaway QEMU VMs (Ubuntu 5.4 kernel, busybox initramfs):

| Configuration | Result |
|---|---|
| Bare metal (Linux), Docker container | `none` |
| KVM, `-cpu host` | `kvm` |
| KVM with Hyper-V enlightenments (`hv-relaxed,hv-vapic,hv-spinlocks`) | `kvm` |
| KVM, hypervisor bit hidden (`-hypervisor,kvm=off`) | `qemu` (DMI) |
| TCG (no KVM) | `qemu` |
| Hidden, SMBIOS `VMware, Inc.` / `VMware7,1` | `vmware` |
| Hidden, SMBIOS `Microsoft Corporation` / `Virtual Machine` | `hyperv` |
| Hidden, SMBIOS `Microsoft Corporation` / `Surface Laptop 5` | `none` |
| Android emulator (API 21, x86_64) | `android-emulator` |

Not tested: Windows on bare metal with VBS (the root partition check), Windows guests, WSL, ARM VMs.

## Limits

- A VM configured to hide itself (hypervisor bit off, real-hardware firmware strings) is reported as `none`.
- ARM and RISC-V have no CPUID: a VM without DMI and without a QEMU/Xen device tree isn't detected.
- Containers are not VMs and report `none`; on a VM host they report what the VM is.

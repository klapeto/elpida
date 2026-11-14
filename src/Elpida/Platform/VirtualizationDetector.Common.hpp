/*
 *  Copyright (c) 2026  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// Shared by VirtualizationDetector.Unix.cpp and VirtualizationDetector.Windows.cpp: the CPUID check (x86) and the
// firmware (DMI/SMBIOS) vendor and product names of known VMs.

#ifndef ELPIDA_PLATFORM_VIRTUALIZATIONDETECTOR_COMMON_HPP_
#define ELPIDA_PLATFORM_VIRTUALIZATIONDETECTOR_COMMON_HPP_

#include <cstring>
#include <string>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#define ELPIDA_VIRTUALIZATION_CPUID 1
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <cpuid.h>
#endif
#endif

namespace Elpida::Virtualization
{
#if defined(ELPIDA_VIRTUALIZATION_CPUID)
	struct CpuidRegisters
	{
		unsigned eax, ebx, ecx, edx;
	};

	// Not __get_cpuid: it checks the leaf against the basic maximum leaf, so it never executes the hypervisor leaves
	// (0x40000000 and up).
	inline CpuidRegisters Cpuid(unsigned leaf)
	{
		CpuidRegisters r{};
#ifdef _MSC_VER
		int regs[4];
		__cpuidex(regs, (int)leaf, 0);
		r = { (unsigned)regs[0], (unsigned)regs[1], (unsigned)regs[2], (unsigned)regs[3] };
#else
		__cpuid_count(leaf, 0, r.eax, r.ebx, r.ecx, r.edx);
#endif
		return r;
	}

	inline std::string HypervisorVendor(unsigned leaf)
	{
		auto r = Cpuid(leaf);
		char vendor[13];
		std::memcpy(vendor, &r.ebx, 4);
		std::memcpy(vendor + 4, &r.ecx, 4);
		std::memcpy(vendor + 8, &r.edx, 4);
		vendor[12] = '\0';
		return vendor;    // stops at the first NUL ("KVMKVMKVM")
	}

	/// The hypervisor CPUID reports, or "" when it doesn't tell: no hypervisor bit (none, or a hidden one) or the
	/// Hyper-V root partition. A Windows host with VBS or the Hyper-V role runs on the hypervisor too, and only the root
	/// partition may create partitions. But a VM with nested virtualization (Azure, Microsoft Dev Box) whose Windows
	/// uses VBS is the root partition of its own nested Hyper-V, so the caller checks the firmware next either way.
	inline std::string DetectFromCpuid()
	{
		if (Cpuid(0).eax < 1 || (Cpuid(1).ecx & (1u << 31)) == 0)
		{
			return {};
		}

		auto maxLeaf = Cpuid(0x40000000).eax;
		auto vendor = HypervisorVendor(0x40000000);

		if (vendor == "Microsoft Hv")
		{
			if (maxLeaf >= 0x40000003 && (Cpuid(0x40000003).ebx & 1u) != 0)
			{
				return {};    // CreatePartitions: the root partition; host or nested, the firmware tells
			}
			// KVM and others with Hyper-V enlightenments report "Microsoft Hv" here and their own id at 0x40000100
			auto second = HypervisorVendor(0x40000100);
			if (second == "KVMKVMKVM")
			{
				return "kvm";
			}
			return "hyperv";
		}
		if (vendor == "KVMKVMKVM" || vendor == "Linux KVM Hv") return "kvm";
		if (vendor == "TCGTCGTCGTCG") return "qemu";
		if (vendor == "VMwareVMware") return "vmware";
		if (vendor == "VBoxVBoxVBox") return "virtualbox";
		if (vendor == "XenVMMXenVMM") return "xen";
		if (vendor == " lrpepyh  vr" || vendor == "prl hyperv  ") return "parallels";
		if (vendor == "bhyve bhyve " || vendor == "BHyVE BHyVE ") return "bhyve";
		if (vendor == "ACRNACRNACRN") return "acrn";
		return "other";
	}
#endif

	inline bool StartsWith(const std::string& value, const char* prefix)
	{
		return value.rfind(prefix, 0) == 0;
	}

	inline bool EndsWith(const std::string& value, const char* suffix)
	{
		auto length = std::strlen(suffix);
		return value.size() >= length && value.compare(value.size() - length, length, suffix) == 0;
	}

	/// The VM that the firmware's system manufacturer and product name (DMI sys_vendor/product_name, SMBIOS type 1)
	/// belong to, or "" for none known. Prefix matches only: these strings are free text on real machines too.
	inline std::string DetectFromFirmware(const std::string& vendor, const std::string& product)
	{
		if (StartsWith(vendor, "QEMU") || StartsWith(product, "QEMU")) return "qemu";
		if (StartsWith(vendor, "KVM") || StartsWith(product, "KVM")) return "kvm";
		if (StartsWith(vendor, "VMware") || StartsWith(product, "VMware")) return "vmware";
		if (StartsWith(vendor, "innotek GmbH") || StartsWith(product, "VirtualBox")) return "virtualbox";
		if (StartsWith(vendor, "Xen") || StartsWith(product, "HVM domU")) return "xen";
		if (StartsWith(vendor, "Parallels") || StartsWith(product, "Parallels")) return "parallels";
		if (StartsWith(vendor, "BHYVE") || StartsWith(product, "BHYVE")) return "bhyve";
		if (StartsWith(vendor, "Bochs") || StartsWith(product, "Bochs")) return "bochs";
		if (StartsWith(product, "Apple Virtualization")) return "apple";
		if (StartsWith(product, "Google Compute Engine")) return "gce";
		if (StartsWith(product, "OpenStack")) return "openstack";
		if (StartsWith(product, "KubeVirt")) return "kubevirt";
		// Bare-metal EC2 instances (*.metal) have the same vendor
		if (StartsWith(vendor, "Amazon EC2") && !EndsWith(product, ".metal")) return "aws";
		// Not the vendor alone: Microsoft's own hardware (Surface) has it too
		if (StartsWith(vendor, "Microsoft Corporation") && StartsWith(product, "Virtual Machine")) return "hyperv";
		return {};
	}

} // Elpida::Virtualization

#endif //ELPIDA_PLATFORM_VIRTUALIZATIONDETECTOR_COMMON_HPP_

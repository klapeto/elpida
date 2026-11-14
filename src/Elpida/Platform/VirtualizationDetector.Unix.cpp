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

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/VirtualizationDetector.hpp"
#include "VirtualizationDetector.Common.hpp"

#include <fstream>
#include <iterator>
#include <string>

#if defined(__ANDROID__)
#include <sys/system_properties.h>
#endif

namespace Elpida
{
	using namespace Virtualization;

	/// The whole file, without the trailing newline; "" if it can't be read. Device tree files are NUL-separated lists.
	static std::string ReadFile(const char* path)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (!file.good())
		{
			return {};
		}
		std::string value((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		while (!value.empty() && (value.back() == '\n' || value.back() == '\0'))
		{
			value.pop_back();
		}
		return value;
	}

	static bool Contains(const std::string& value, const char* text)
	{
		return value.find(text) != std::string::npos;
	}

#if defined(__ANDROID__)
	static std::string GetProperty(const char* name)
	{
		char value[PROP_VALUE_MAX] = {};
		__system_property_get(name, value);
		return value;
	}
#endif

	String VirtualizationDetector::Detect()
	{
#if defined(__ANDROID__)
		// The Android emulator (QEMU based); on an x86 host it would otherwise show up as kvm
		if (GetProperty("ro.kernel.qemu") == "1" || GetProperty("ro.boot.qemu") == "1")
		{
			return "android-emulator";
		}
		auto hardware = GetProperty("ro.hardware");
		if (hardware == "ranchu" || hardware == "goldfish")
		{
			return "android-emulator";
		}
#endif

		// WSL 2 runs in a Hyper-V VM; its kernel release says so (e.g. "6.6.87.2-microsoft-standard-WSL2")
		auto release = ReadFile("/proc/sys/kernel/osrelease");
		if (Contains(release, "microsoft") || Contains(release, "Microsoft") || Contains(release, "WSL"))
		{
			return "wsl";
		}

#if defined(ELPIDA_VIRTUALIZATION_CPUID)
		auto fromCpuid = DetectFromCpuid();
		if (!fromCpuid.empty())
		{
			return fromCpuid;
		}
#endif

		// Xen guests (also paravirtualized ones, which may not have the CPUID bit)
		auto hypervisorType = ReadFile("/sys/hypervisor/type");
		if (!hypervisorType.empty())
		{
			return hypervisorType == "xen" ? "xen" : "other";
		}

		auto fromFirmware = DetectFromFirmware(ReadFile("/sys/class/dmi/id/sys_vendor"),
				ReadFile("/sys/class/dmi/id/product_name"));
		if (!fromFirmware.empty())
		{
			return fromFirmware;
		}

		// Device tree (ARM, RISC-V) machines without DMI
		if (Contains(ReadFile("/proc/device-tree/hypervisor/compatible"), "xen"))
		{
			return "xen";
		}
		if (Contains(ReadFile("/proc/device-tree/compatible"), "linux,dummy-virt"))
		{
			return "qemu";    // QEMU's "virt" machine
		}

		return "none";
	}
} // Elpida

#endif // defined(ELPIDA_UNIX)

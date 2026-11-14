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

#ifndef ELPIDA_PLATFORM_VIRTUALIZATIONDETECTOR_HPP_
#define ELPIDA_PLATFORM_VIRTUALIZATIONDETECTOR_HPP_

#include "Elpida/Core/String.hpp"

namespace Elpida
{
	/// Best-effort detection of a virtual machine (docs/development/virtualization-detection.md). Results from VMs are
	/// uploaded with this flag and kept out of the statistics; a VM that hides itself is not detected.
	class VirtualizationDetector final
	{
	 public:
		/// "none" on bare metal (including a Windows host that runs on Hyper-V for VBS), otherwise a lowercase id:
		/// kvm, qemu, hyperv, wsl, vmware, virtualbox, xen, parallels, bhyve, acrn, apple, aws, gce, openstack,
		/// kubevirt, bochs, android-emulator, or "other" for an unknown hypervisor.
		[[nodiscard]]
		static String Detect();
	};

} // Elpida

#endif //ELPIDA_PLATFORM_VIRTUALIZATIONDETECTOR_HPP_

// =========================================================================
//
// Elpida Mobile
//
// Copyright (C) 2026 Ioannis Panagiotopoulos
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// =========================================================================

namespace Elpida.Mobile.Models
{
	/// <param name="Allowed">CPUs the benchmark processes are permitted to run on (cpuset/affinity)</param>
	/// <param name="Online">CPUs online right now, -1 if unknown</param>
	/// <param name="Possible">CPUs of the device, including offline ones, -1 if unknown</param>
	public record CpuAvailability(int Allowed, int Online, int Possible)
	{
		/// <summary>
		/// Android keeps online CPUs away from the app (e.g. not on screen, or a restricted cpuset). Offline and paused
		/// CPUs are not counted: power management parks cores while idle and brings them back under load.
		/// </summary>
		public bool IsRestricted => Online > 0 && Allowed < Online;
	}
}

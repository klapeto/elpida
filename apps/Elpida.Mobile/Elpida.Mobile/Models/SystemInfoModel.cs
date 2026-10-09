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
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//
// You should have received a copy of the GNU General Public License
// =========================================================================

namespace Elpida.Mobile.Models
{
	public class SystemInfoModel
	{
		public CpuInfoModel Cpu { get; set; } = new ();

		public MemoryInfoModel Memory { get; set; } = new ();

		public OsInfoModel Os { get; set; } = new ();

		public TopologyModel Topology { get; set; } = new ();

		public ElpidaVersionModel ElpidaVersion { get; set; } = new ();

		public TimingModel Timing { get; set; } = new ();
	}
}
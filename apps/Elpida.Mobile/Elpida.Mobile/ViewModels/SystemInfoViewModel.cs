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

using CommunityToolkit.Mvvm.ComponentModel;
using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public partial class SystemInfoViewModel : ObservableObject
	{
		[ObservableProperty]
		public bool _loaded;

		public SystemInfoViewModel(SystemInfoModel model)
		{
			CpuInfo = new CpuInfoViewModel(model.Cpu);
			MemoryInfo = new MemoryInfoViewModel(model.Memory);
			OsInfo = new OsInfoViewModel(model.Os);
			TopologyInfo = new TopologyViewModel(model.Topology);
			TimingInfo = new TimingViewModel(model.Timing);
		}

		public CpuInfoViewModel CpuInfo { get; }

		public MemoryInfoViewModel MemoryInfo { get; }

		public OsInfoViewModel OsInfo { get; }

		public TopologyViewModel TopologyInfo { get; }

		public TimingViewModel TimingInfo { get; }
	}
}
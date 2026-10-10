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

using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public class TopologyNodeViewModel
	{
		private static readonly Dictionary<TopologyNodeType, string> NodeNames = new()
		{
			[TopologyNodeType.Machine] = Resources.Machine,
			[TopologyNodeType.Package] = Resources.Package,
			[TopologyNodeType.NumaDomain] = Resources.NumaDomain,
			[TopologyNodeType.Group] = Resources.Group,
			[TopologyNodeType.Die] = Resources.Die,
			[TopologyNodeType.Core] = Resources.Core,
			[TopologyNodeType.L1ICache] = Resources.L1ICache,
			[TopologyNodeType.L1DCache] = Resources.L1DCache,
			[TopologyNodeType.L2ICache] = Resources.L2ICache,
			[TopologyNodeType.L2DCache] = Resources.L2DCache,
			[TopologyNodeType.L3ICache] = Resources.L3ICache,
			[TopologyNodeType.L3DCache] = Resources.L3DCache,
			[TopologyNodeType.L4Cache] = Resources.L4Cache,
			[TopologyNodeType.L5Cache] = Resources.L5Cache,
			[TopologyNodeType.ProcessingUnit] = Resources.ProcessingUnit,
		};

		private static readonly Dictionary<TopologyNodeType, Color> NodeColors = new()
		{
			[TopologyNodeType.Machine] = Color.Parse("#aebfef"),
			[TopologyNodeType.Package] = Color.Parse("#99da9f"),
			[TopologyNodeType.NumaDomain] = Color.Parse("#d4d4a1"),
			[TopologyNodeType.Group] = Color.Parse("#db8ced"),
			[TopologyNodeType.Die] = Color.Parse("#a9a9a9"),
			[TopologyNodeType.Core] = Color.Parse("#9297e3"),
			[TopologyNodeType.L1ICache] = Color.Parse("#f5bfbf"),
			[TopologyNodeType.L1DCache] = Color.Parse("#fa9e9e"),
			[TopologyNodeType.L2ICache] = Color.Parse("#e6abab"),
			[TopologyNodeType.L2DCache] = Color.Parse("#e48b8b"),
			[TopologyNodeType.L3ICache] = Color.Parse("#c68989"),
			[TopologyNodeType.L3DCache] = Color.Parse("#c67171"),
			[TopologyNodeType.L4Cache] = Color.Parse("#ae5151"),
			[TopologyNodeType.L5Cache] = Color.Parse("#8f4242"),
			[TopologyNodeType.ProcessingUnit] = Color.Parse("#c8caea"),
		};

		public TopologyNodeViewModel()
		{
			Type = TopologyNodeType.Machine;
			Children = new List<TopologyNodeViewModel>();
			MemoryChildren = new List<TopologyNodeViewModel>();
		}

		public TopologyNodeViewModel(TopologyNodeModel model)
		{
			Type = model.Type;
			OsIndex = model.OsIndex;
			Size = model.Size;
			Children = model.Children.Select(c => new TopologyNodeViewModel(c)).ToList();
			MemoryChildren = model.MemoryChildren.Select(c => new TopologyNodeViewModel(c)).ToList();
		}

		public Color NodeColor => NodeColors[Type];

		public string NodeName => NodeNames[Type];

		public bool ShowSize => Size.HasValue;

		public bool ShowIndex => Type == TopologyNodeType.ProcessingUnit;

		public List<TopologyNodeViewModel> Children { get; }

		public List<TopologyNodeViewModel> MemoryChildren { get; }

		public TopologyNodeType Type { get; }

		public ulong? OsIndex { get; }

		public ulong? Size { get; }
	}
}
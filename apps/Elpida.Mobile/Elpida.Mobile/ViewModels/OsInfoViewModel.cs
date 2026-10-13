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
	public class OsInfoViewModel
	{
		public OsInfoViewModel()
		{
			Category = string.Empty;
			Name = string.Empty;
			Version = string.Empty;
		}

		public OsInfoViewModel(OsInfoModel model)
		{
			Category = model.Category;
			Name = model.Name;
			Version = model.Version;
		}

		public string Category { get; }

		public string Name { get; }

		public string Version { get; }
	}
}
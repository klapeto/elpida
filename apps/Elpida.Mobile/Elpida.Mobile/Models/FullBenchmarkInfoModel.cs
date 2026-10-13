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
	public class FullBenchmarkInfoModel
	{
		public string Name { get; set; } = string.Empty;

		public string Description { get; set; } = string.Empty;

		public string ResultUnit { get; set; } = string.Empty;

		public ResultType ResultType { get; set; } = ResultType.Custom;

		public string Filename { get; set; } = string.Empty;

		public int Index { get; set; }

		public List<BenchmarkConfigurationModel> Configurations { get; set; } = [];
	}
}
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

namespace Elpida.Mobile.Services
{
	public class SettingsService
	{
		public string ApiHost { get; } = "http://10.0.2.2:5000";

		public string ApiKey { get; } = "__TestAPIKey__";

		public bool BenchmarkNotificationIssued
		{
			get => Preferences.Default.Get("benchmark_confirmed", false);
			set => Preferences.Default.Set("benchmark_confirmed", value);
		}
	}
}
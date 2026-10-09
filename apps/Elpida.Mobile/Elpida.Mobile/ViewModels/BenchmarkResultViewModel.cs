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
using CommunityToolkit.Mvvm.Input;

namespace Elpida.Mobile.ViewModels
{
	public partial class BenchmarkResultViewModel : ObservableObject
	{
		[ObservableProperty]
		private bool _isTaskResultsExpanded;

		public ResultViewModel TotalScore { get; set; } = new ();

		public ResultViewModel SingleThreadScore { get; set; } = new ();

		public ResultViewModel MultiThreadScore { get; set; } = new ();

		public List<BenchmarkTaskResultViewModel> TaskResults { get; set; } = new ();

		public string TaskResultsToggleText =>
			IsTaskResultsExpanded ? Resources.HideBenchmarkDetails : Resources.ShowBenchmarkDetails;

		[RelayCommand]
		private void ToggleTaskResults()
		{
			IsTaskResultsExpanded = !IsTaskResultsExpanded;
		}

		partial void OnIsTaskResultsExpandedChanged(bool value)
		{
			OnPropertyChanged(nameof(TaskResultsToggleText));
		}
	}
}
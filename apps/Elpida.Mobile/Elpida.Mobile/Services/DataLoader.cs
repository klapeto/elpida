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

using Elpida.Mobile.ViewModels;

namespace Elpida.Mobile.Services
{
	public class DataLoader
	{
		private readonly BenchmarkPageViewModel _benchmarkPageViewModel;
		private readonly MainPageViewModel _mainPageViewModel;
		private readonly MessageService _messageService;
		private readonly ElpidaService _service;

		public DataLoader(
			ElpidaService elpidaService,
			MainPageViewModel mainPageViewModel,
			BenchmarkPageViewModel benchmarkPageViewModel,
			MessageService messageService
		)
		{
			_mainPageViewModel = mainPageViewModel;
			_benchmarkPageViewModel = benchmarkPageViewModel;
			_messageService = messageService;
			_service = elpidaService;
		}

		public async Task LoadInitialDataAsync()
		{
			try
			{
				var info = await _service.LoadAsync();
				_mainPageViewModel.SystemInfo = new SystemInfoViewModel(info)
				{
					Loaded = true,
				};

				_benchmarkPageViewModel.BenchmarksInstancesModels = [.. info.Benchmarks];
				_benchmarkPageViewModel.Loaded = true;
			}
			catch (Exception e)
			{
				await _messageService.DisplayAlertAsync("Error", $"Failed to load: {e}", "OK").ConfigureAwait(false);
			}
		}
	}
}
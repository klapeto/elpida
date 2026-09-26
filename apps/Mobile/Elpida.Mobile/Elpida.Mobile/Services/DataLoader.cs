using System.Diagnostics;
using Elpida.Mobile.ViewModels;

namespace Elpida.Mobile.Services
{
	public class DataLoader
	{
		private readonly MainPageViewModel _mainPageViewModel;
		private readonly BenchmarkPageViewModel _benchmarkPageViewModel;
		private readonly ElpidaService _service;
		private readonly MessageService _messageService;

		public DataLoader(ElpidaService elpidaService, MainPageViewModel mainPageViewModel,
			BenchmarkPageViewModel benchmarkPageViewModel, MessageService messageService)
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
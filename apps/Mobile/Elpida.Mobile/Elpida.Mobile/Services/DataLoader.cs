using System.Diagnostics;
using Elpida.Mobile.ViewModels;

namespace Elpida.Mobile.Services
{
	public class DataLoader
	{
		private readonly MainPageViewModel _mainPageViewModel;
		private readonly BenchmarkPageViewModel _benchmarkPageViewModel;
		private readonly ElpidaService _service;

		public DataLoader(ElpidaService elpidaService, MainPageViewModel mainPageViewModel,
			BenchmarkPageViewModel benchmarkPageViewModel)
		{
			_mainPageViewModel = mainPageViewModel;
			_benchmarkPageViewModel = benchmarkPageViewModel;
			_service = elpidaService;
		}

		public async Task LoadInitialDataAsync()
		{
			var info = await _service.LoadAsync();
			_mainPageViewModel.SystemInfo = new SystemInfoViewModel(info)
			{
				Loaded = true,
			};

			_benchmarkPageViewModel.BenchmarksInstancesModels = [.. info.Benchmarks];
			_benchmarkPageViewModel.Loaded = true;
		}
	}
}
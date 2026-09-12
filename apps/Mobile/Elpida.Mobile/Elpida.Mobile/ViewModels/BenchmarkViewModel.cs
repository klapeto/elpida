using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Elpida.Mobile.Models;
using Elpida.Mobile.Services;

namespace Elpida.Mobile.ViewModels
{
	public partial class BenchmarkPageViewModel : ObservableObject
	{
		[ObservableProperty]
		public int _benchmarkCount;

		[ObservableProperty]
		public TimeSpan _estimatedTime;

		[ObservableProperty]
		public int _executedBenchmarks;

		[ObservableProperty]
		public BenchmarkInfoViewModel? _executingBenchmark;

		[ObservableProperty]
		public BenchmarkResultViewModel? _lastResult;

		[ObservableProperty]
		public bool _loaded;

		[ObservableProperty]
		public int _runItTimes;

		[ObservableProperty]
		public bool _running;

		[ObservableProperty]
		public bool _uploadResults = true;

		private ElpidaService _elpidaService;

		public BenchmarkPageViewModel(ElpidaService elpidaService)
		{
			_elpidaService = elpidaService;
		}

		public List<FullBenchmarkInstanceModel> BenchmarksInstancesModels { get; set; } = new();

		[RelayCommand(CanExecute = nameof(CanRun))]
		public async Task RunBenchmark()
		{
			if (Running) return;

			try
			{
				Running = true;
				BenchmarkCount = BenchmarksInstancesModels.Count;
				ExecutedBenchmarks = 0;


				for (var index = 0; index < BenchmarksInstancesModels.Count; index++)
				{
					var fullBenchmarkInstanceModel = BenchmarksInstancesModels[index];
					var result = await _elpidaService.RunBenchmarkAsync(index);
				}
			}
			finally
			{
				Running = false;
			}
		}

		public bool CanRun()
		{
			return !Running;
		}
	}
}
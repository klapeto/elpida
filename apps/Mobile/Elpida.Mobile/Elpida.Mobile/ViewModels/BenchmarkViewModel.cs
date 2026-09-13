using System.Collections.ObjectModel;
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
		public int _runItTimes = 1;

		[ObservableProperty]
		public bool _running;

		[ObservableProperty]
		public bool _uploadResults = true;

		private ElpidaService _elpidaService;

		[ObservableProperty]
		public double _progress;
		
		[ObservableProperty]
		public ObservableCollection<BenchmarkResultViewModel> _benchmarkResults = new ObservableCollection<BenchmarkResultViewModel>();

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
				RunItTimes = Math.Clamp(RunItTimes, 1, 10);
				Running = true;
				BenchmarkCount = BenchmarksInstancesModels.Count * RunItTimes;
				ExecutedBenchmarks = 0;

				var singleThreadScores = new List<(double, double)>();
				var multiThreadScores = new List<(double, double)>();
				for (var index = 0; index < BenchmarksInstancesModels.Count; index++)
				{
					var fullBenchmarkInstanceModel = BenchmarksInstancesModels[index];
					ExecutingBenchmark = new BenchmarkInfoViewModel
					{
						Name = fullBenchmarkInstanceModel.Name
					};
					ExecutedBenchmarks++;
					Progress = ExecutedBenchmarks / (double)BenchmarkCount;
					var result = await _elpidaService.RunBenchmarkAsync(index);
					// await Task.Delay(500);
					//var result = new Random().Next(100);
		
					if (fullBenchmarkInstanceModel.IsMultiThread)
					{
						multiThreadScores.Add((result, fullBenchmarkInstanceModel.BaseScore));
					}
					else
					{
						singleThreadScores.Add((result, fullBenchmarkInstanceModel.BaseScore));
					}
				}
				
				var singleTheadScore = ElpidaService.CalculateScore(singleThreadScores.Select(x => x.Item1).ToArray(), singleThreadScores.Select(x => x.Item2).ToArray(), singleThreadScores.Count);
				var multiThreadScore = ElpidaService.CalculateScore(multiThreadScores.Select(x => x.Item1).ToArray(), multiThreadScores.Select(x => x.Item2).ToArray(), multiThreadScores.Count);
				var totalScore = ElpidaService.CalculateTotalScore(singleTheadScore, multiThreadScore);
				var resultViewModel = new BenchmarkResultViewModel
				{
					SingleThreadScore = new ResultViewModel(){Value = singleTheadScore},
					MultiThreadScore = new ResultViewModel(){Value = multiThreadScore},
					TotalScore = new ResultViewModel(){Value = totalScore}
				};
				if (LastResult != null)
				{
					resultViewModel.TotalScore.RelativeChange =
						((totalScore - LastResult.TotalScore.Value) / LastResult.TotalScore.Value) * 100;
					resultViewModel.SingleThreadScore.RelativeChange =
						((singleTheadScore - LastResult.SingleThreadScore.Value) / LastResult.SingleThreadScore.Value) *
						100;
					resultViewModel.MultiThreadScore.RelativeChange =
						((multiThreadScore - LastResult.MultiThreadScore.Value) / LastResult.MultiThreadScore.Value) *
						100;
				}

				LastResult = resultViewModel;
				BenchmarkResults.Add(LastResult);
			}
			finally
			{
				Running = false;
				ExecutingBenchmark = null;
			}
		}

		public bool CanRun()
		{
			return !Running;
		}
	}
}
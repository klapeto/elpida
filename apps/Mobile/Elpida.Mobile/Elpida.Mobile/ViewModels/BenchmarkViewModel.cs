using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Elpida.Mobile.Models;
using Elpida.Mobile.Models.Web;
using Elpida.Mobile.Services;

namespace Elpida.Mobile.ViewModels
{
	public partial class BenchmarkPageViewModel : ObservableObject
	{
		[ObservableProperty]
		public int _benchmarkCount;

		[ObservableProperty]
		public ObservableCollection<BenchmarkResultViewModel> _benchmarkResults = new();

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
		public double _progress;

		[ObservableProperty]
		public int _runItTimes = 1;

		[ObservableProperty]
		public bool _running;

		[ObservableProperty]
		public bool _uploadResults = true;

		private readonly CancellationTokenSource _cancel = new();
		private readonly MessageService _messageService;
		private readonly SettingService _settingService;
		private readonly UploadService _uploadService;

		[ObservableProperty]
		private string _buttonText = "Start";

		private ElpidaService _elpidaService;
		private bool _enteredEnergySaveMode;

		public BenchmarkPageViewModel(ElpidaService elpidaService, UploadService uploadService,
			MessageService messageService, SettingService settingService)
		{
			_elpidaService = elpidaService;
			_uploadService = uploadService;
			_messageService = messageService;
			_settingService = settingService;
		}

		public List<FullBenchmarkInstanceModel> BenchmarksInstancesModels { get; set; } = new();

		[RelayCommand(AllowConcurrentExecutions = true)]
		public async Task RunBenchmark()
		{
			try
			{
				if (!_settingService.BenchmarkNotificationIssued)
				{
					await _messageService.DisplayAlertAsync("Information",
						"Running benchmarks will make your device warm. Also keep you device charged, and on power if possible.",
						"OK");
					_settingService.BenchmarkNotificationIssued = true;
				}

				if (Running)
				{
					if (_cancel.Token.IsCancellationRequested)
					{
						await _cancel.CancelAsync();
						ExecutingBenchmark = new BenchmarkInfoViewModel
						{
							Name = "Waiting for cancellation...",
						};
						ButtonText = "Canceling...";
					}

					return;
				}

				RunItTimes = Math.Clamp(RunItTimes, 1, 10);
				Running = true;
				ButtonText = "Stop";
				BenchmarkCount = BenchmarksInstancesModels.Count * RunItTimes;
				ExecutedBenchmarks = 0;
				_enteredEnergySaveMode = false;
				DeviceDisplay.Current.KeepScreenOn = true;
				Battery.Default.EnergySaverStatusChanged += DefaultOnEnergySaverStatusChanged;
				var resultList = new List<ResultDto>();

				for (var i = 0; i < RunItTimes; i++)
				{
					var runResults = new List<ResultBenchmarkResultDto>();

					var singleThreadScores = new List<(double, double)>();
					var multiThreadScores = new List<(double, double)>();
					for (var index = 0; index < BenchmarksInstancesModels.Count; index++)
					{
						if (_cancel.Token.IsCancellationRequested) return;

						var fullBenchmarkInstanceModel = BenchmarksInstancesModels[index];
						ExecutingBenchmark = new BenchmarkInfoViewModel
						{
							Name = fullBenchmarkInstanceModel.Name,
						};
						ExecutedBenchmarks++;
						Progress = ExecutedBenchmarks / (double)BenchmarkCount;
						var result =
							new Random().Next(90, 100); //await _elpidaService.RunBenchmarkAsync(index, _cancel.Token);
						runResults.Add(new ResultBenchmarkResultDto
						{
							Result = result,
							Uuid = fullBenchmarkInstanceModel.Uuid,
						});

						if (fullBenchmarkInstanceModel.IsMultiThread)
							multiThreadScores.Add((result, fullBenchmarkInstanceModel.BaseScore));
						else
							singleThreadScores.Add((result, fullBenchmarkInstanceModel.BaseScore));
					}

					var singleTheadScore = ElpidaService.CalculateScore(
						singleThreadScores.Select(x => x.Item1).ToArray(),
						singleThreadScores.Select(x => x.Item2).ToArray(), singleThreadScores.Count);
					var multiThreadScore = ElpidaService.CalculateScore(
						multiThreadScores.Select(x => x.Item1).ToArray(),
						multiThreadScores.Select(x => x.Item2).ToArray(), multiThreadScores.Count);
					var totalScore = ElpidaService.CalculateTotalScore(singleTheadScore, multiThreadScore);
					var resultViewModel = new BenchmarkResultViewModel
					{
						SingleThreadScore = new ResultViewModel { Value = singleTheadScore },
						MultiThreadScore = new ResultViewModel { Value = multiThreadScore },
						TotalScore = new ResultViewModel { Value = totalScore },
					};
					if (LastResult != null)
					{
						resultViewModel.TotalScore.RelativeChange =
							(totalScore - LastResult.TotalScore.Value) / LastResult.TotalScore.Value * 100;
						resultViewModel.SingleThreadScore.RelativeChange =
							(singleTheadScore - LastResult.SingleThreadScore.Value) /
							LastResult.SingleThreadScore.Value *
							100;
						resultViewModel.MultiThreadScore.RelativeChange =
							(multiThreadScore - LastResult.MultiThreadScore.Value) /
							LastResult.MultiThreadScore.Value *
							100;
					}

					LastResult = resultViewModel;
					BenchmarkResults.Add(LastResult);
					resultList.Add(new ResultDto
					{
						TimeStamp = DateTime.UtcNow,
						TotalScore = totalScore,
						SingleThreadScore = singleTheadScore,
						MultiThreadScore = multiThreadScore,
						BenchmarkResults = runResults.ToArray(),
					});
				}

				if (UploadResults && Battery.Default.EnergySaverStatus != EnergySaverStatus.On &&
				    !_enteredEnergySaveMode) await _uploadService.UploadResultsAsync(resultList);
			}
			catch (OperationCanceledException)
			{
				// ignored
			}
			catch (HttpRequestException e)
			{
				throw new ApplicationException("Failed to upload results", e);
			}
			finally
			{
				DeviceDisplay.Current.KeepScreenOn = false;
				Battery.Default.EnergySaverStatusChanged -= DefaultOnEnergySaverStatusChanged;
				_enteredEnergySaveMode = false;
				Running = false;
				ExecutingBenchmark = null;
				ExecutedBenchmarks = 0;
				_cancel.TryReset();
				ButtonText = "Start";
			}
		}

		private void DefaultOnEnergySaverStatusChanged(object? sender, EnergySaverStatusChangedEventArgs e)
		{
			_enteredEnergySaveMode = e.EnergySaverStatus == EnergySaverStatus.On;
		}
	}
}
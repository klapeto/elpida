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
		private readonly ElpidaService _elpidaService;
		private readonly MessageService _messageService;
		private readonly SettingsService _settingsService;
		private readonly UploadService _uploadService;

		[ObservableProperty]
		private int _benchmarkCount;

		[ObservableProperty]
		private ObservableCollection<BenchmarkResultViewModel> _benchmarkResults = new();

		[ObservableProperty]
		private string _buttonText = Resources.Start;

		private CancellationTokenSource _cancel = new();
		private bool _enteredEnergySaveMode;
		private bool _leftForeground;
		private CpuAvailability? _restrictedCpus;
		private Microsoft.Maui.Controls.Window? _window;

		[ObservableProperty]
		private TimeSpan _estimatedTime;

		[ObservableProperty]
		private int _executedBenchmarks;

		[ObservableProperty]
		private BenchmarkInfoViewModel? _executingBenchmark;

		[ObservableProperty]
		private BenchmarkResultViewModel? _lastResult;

		[ObservableProperty]
		private bool _loaded;

		[ObservableProperty]
		private double _progress;

		[ObservableProperty]
		private int _runItTimes = 1;

		[ObservableProperty]
		private bool _running;

		[ObservableProperty]
		private bool _uploadResults = true;

		public BenchmarkPageViewModel(
			ElpidaService elpidaService,
			UploadService uploadService,
			MessageService messageService,
			SettingsService settingsService
		)
		{
			_elpidaService = elpidaService;
			_uploadService = uploadService;
			_messageService = messageService;
			_settingsService = settingsService;
			OnRunItTimesChanged(_runItTimes);
		}

		public List<FullBenchmarkInstanceModel> BenchmarksInstancesModels { get; set; } = new();

		[RelayCommand(AllowConcurrentExecutions = true)]
		public async Task RunBenchmark()
		{
			if (!_settingsService.BenchmarkNotificationIssued)
			{
				await _messageService.DisplayAlertAsync(
					Resources.Information,
					Resources.BenchmarkWarning,
					Resources.Ok
				);

				_settingsService.BenchmarkNotificationIssued = true;
			}

			if (Running)
			{
				if (!_cancel.Token.IsCancellationRequested)
				{
					await _cancel.CancelAsync();
					ExecutingBenchmark = new BenchmarkInfoViewModel
					{
						Name = Resources.WaitingForCancellation,
					};

					ButtonText = Resources.Cancelling;
				}

				return;
			}

			// Android limits the CPUs of apps per state (cpusets): fewer cores would make the results lower
			var cpus = TryGetCpuAvailability();
			if (cpus is { IsRestricted: true }
			    && !await _messageService.DisplayAlertAsync(
				    Resources.Information,
				    string.Format(Resources.CpuRestrictedWarning, cpus.Allowed, cpus.Online),
				    Resources.Continue,
				    Resources.Cancel
			    ))
			{
				return;
			}

			try
			{
				Running = true;
				_restrictedCpus = cpus is { IsRestricted: true } ? cpus : null;
				_leftForeground = false;
				_window = Microsoft.Maui.Controls.Application.Current?.Windows.FirstOrDefault();
				if (_window != null)
				{
					_window.Stopped += WindowOnStopped;
				}

				ButtonText = Resources.Stop;
				BenchmarkCount = BenchmarksInstancesModels.Count * RunItTimes;
				ExecutedBenchmarks = 0;
				_enteredEnergySaveMode = false;
				DeviceDisplay.Current.KeepScreenOn = true;
				Battery.Default.EnergySaverStatusChanged += DefaultOnEnergySaverStatusChanged;
				var resultList = new List<ResultDto>();

				for (var i = 0; i < RunItTimes; i++)
				{
					var runResults = new List<ResultBenchmarkResultDto>();
					var taskResults = new List<BenchmarkTaskResultViewModel>();

					var singleThreadScores = new List<(double, double)>();
					var multiThreadScores = new List<(double, double)>();
					for (var index = 0; index < BenchmarksInstancesModels.Count; index++)
					{
						if (_cancel.Token.IsCancellationRequested)
						{
							return;
						}

						var fullBenchmarkInstanceModel = BenchmarksInstancesModels[index];
						ExecutingBenchmark = new BenchmarkInfoViewModel
						{
							Name = fullBenchmarkInstanceModel.Name,
						};

						Progress = ExecutedBenchmarks / (double)BenchmarkCount;
						CheckCpuAvailability();
						var result = await _elpidaService.RunBenchmarkAsync(index, _cancel.Token);
						CheckCpuAvailability();
						ExecutedBenchmarks++;
						runResults.Add(
							new ResultBenchmarkResultDto
							{
								Result = result,
								Uuid = fullBenchmarkInstanceModel.Uuid,
							}
						);

						taskResults.Add(
							new BenchmarkTaskResultViewModel
							{
								Name = fullBenchmarkInstanceModel.Name,
								Value = result,
								Unit =
									$"{fullBenchmarkInstanceModel.BenchmarkInfo.ResultUnit}{(fullBenchmarkInstanceModel.BenchmarkInfo.ResultType == ResultType.Throughput ? "/s" : string.Empty)}",
							}
						);

						if (fullBenchmarkInstanceModel.IsMultiThread)
						{
							multiThreadScores.Add((result, fullBenchmarkInstanceModel.BaseScore));
						}
						else
						{
							singleThreadScores.Add((result, fullBenchmarkInstanceModel.BaseScore));
						}
					}

					var singleTheadScore = ElpidaService.CalculateScore(
						singleThreadScores.Select(x => x.Item1).ToArray(),
						singleThreadScores.Select(x => x.Item2).ToArray(),
						singleThreadScores.Count
					);

					var multiThreadScore = ElpidaService.CalculateScore(
						multiThreadScores.Select(x => x.Item1).ToArray(),
						multiThreadScores.Select(x => x.Item2).ToArray(),
						multiThreadScores.Count
					);

					var totalScore = ElpidaService.CalculateTotalScore(singleTheadScore, multiThreadScore);
					var resultViewModel = new BenchmarkResultViewModel
					{
						SingleThreadScore = new ResultViewModel { Value = singleTheadScore },
						MultiThreadScore = new ResultViewModel { Value = multiThreadScore },
						TotalScore = new ResultViewModel { Value = totalScore },
						TaskResults = taskResults,
					};

					if (LastResult != null)
					{
						resultViewModel.TotalScore.RelativeChange =
							((totalScore - LastResult.TotalScore.Value) / LastResult.TotalScore.Value) * 100;

						resultViewModel.SingleThreadScore.RelativeChange =
							((singleTheadScore - LastResult.SingleThreadScore.Value)
							 / LastResult.SingleThreadScore.Value)
							* 100;

						resultViewModel.MultiThreadScore.RelativeChange =
							((multiThreadScore - LastResult.MultiThreadScore.Value) / LastResult.MultiThreadScore.Value)
							* 100;
					}

					LastResult = resultViewModel;
					BenchmarkResults.Add(LastResult);
					resultList.Add(
						new ResultDto
						{
							TimeStamp = DateTime.UtcNow,
							TotalScore = totalScore,
							SingleThreadScore = singleTheadScore,
							MultiThreadScore = multiThreadScore,
							BenchmarkResults = runResults.ToArray(),
						}
					);
				}

				if (UploadResults
				    && Battery.Default.EnergySaverStatus != EnergySaverStatus.On
				    && !_enteredEnergySaveMode
				    && _restrictedCpus == null
				    && !_leftForeground)
				{
					await _uploadService.UploadResultsAsync(resultList);
				}
				else if (UploadResults && (_restrictedCpus != null || _leftForeground))
				{
					await _messageService.DisplayAlertAsync(
						Resources.Information,
						_restrictedCpus != null
							? string.Format(
								Resources.ResultsNotUploadedCpuRestricted,
								_restrictedCpus.Allowed,
								_restrictedCpus.Online
							)
							: Resources.ResultsNotUploadedBackground,
						Resources.Ok
					);
				}
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
				if (_window != null)
				{
					_window.Stopped -= WindowOnStopped;
					_window = null;
				}
				ExecutingBenchmark = null;
				ExecutedBenchmarks = 0;
				_cancel.Dispose();
				_cancel = new CancellationTokenSource();
				ButtonText = Resources.Start;
				Running = false;
			}
		}

		partial void OnRunItTimesChanged(int value)
		{
			EstimatedTime = TimeSpan.FromMinutes(value * 3.3);
			if (value <= 0)
			{
				RunItTimes = 1;
			}
		}

		private void DefaultOnEnergySaverStatusChanged(object? sender, EnergySaverStatusChangedEventArgs e)
		{
			_enteredEnergySaveMode = e.EnergySaverStatus == EnergySaverStatus.On;
		}

		private void WindowOnStopped(object? sender, EventArgs e)
		{
			// Not on screen anymore: Android moves the app (and the benchmark processes) to a restricted cpuset
			_leftForeground = true;
		}

		/// <summary>
		/// Checked before and after every benchmark; a restriction during the run keeps the results from being uploaded.
		/// </summary>
		private void CheckCpuAvailability()
		{
			var cpus = TryGetCpuAvailability();
			if (cpus is { IsRestricted: true } && (_restrictedCpus == null || cpus.Allowed < _restrictedCpus.Allowed))
			{
				_restrictedCpus = cpus;
			}
		}

		private static CpuAvailability? TryGetCpuAvailability()
		{
			try
			{
				return ElpidaService.GetCpuAvailability();
			}
			catch (Exception)
			{
				// Unknown: don't block the benchmark
				return null;
			}
		}
	}
}
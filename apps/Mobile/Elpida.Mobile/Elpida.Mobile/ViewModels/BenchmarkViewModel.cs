using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace Elpida.Mobile.ViewModels
{
	public partial class BenchmarkPageViewModel : ObservableObject
	{
		[ObservableProperty]
		public BenchmarkResultViewModel? _lastResult;
		
		[ObservableProperty]
		public BenchmarkInfoViewModel? _executingBenchmark;
		
		[ObservableProperty]
		public int _benchmarkCount;

		[ObservableProperty]
		public int _executedBenchmarks;
		
		[ObservableProperty]
		public int _runItTimes;
		
		[ObservableProperty]
		public bool _uploadResults = true;
		
		[ObservableProperty]
		public TimeSpan _estimatedTime;

		[RelayCommand]
		public async Task RunBenchmark()
		{
			
		}
	}
}
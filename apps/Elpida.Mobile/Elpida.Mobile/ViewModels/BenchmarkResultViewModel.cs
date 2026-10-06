using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace Elpida.Mobile.ViewModels
{
	public partial class BenchmarkResultViewModel: ObservableObject
	{
		public ResultViewModel TotalScore { get; set; } = new();
		
		public ResultViewModel SingleThreadScore { get; set; } = new();
		public ResultViewModel MultiThreadScore { get; set; } = new();
		public List<BenchmarkTaskResultViewModel> TaskResults { get; set; } = new();

		[ObservableProperty]
		private bool _isTaskResultsExpanded;

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
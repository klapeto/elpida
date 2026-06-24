using CommunityToolkit.Mvvm.ComponentModel;

namespace Elpida.Mobile.ViewModels
{
	public class BenchmarkResultViewModel: ObservableObject
	{
		public double TotalScore { get; set; }
		public double SingleThreadScore { get; set; }
		public double MultiThreadScore { get; set; }
	}
}
using CommunityToolkit.Mvvm.ComponentModel;

namespace Elpida.Mobile.ViewModels
{
	public class BenchmarkResultViewModel: ObservableObject
	{
		public ResultViewModel TotalScore { get; set; } = new();
		
		public ResultViewModel SingleThreadScore { get; set; } = new();
		public ResultViewModel MultiThreadScore { get; set; } = new();
	}
}
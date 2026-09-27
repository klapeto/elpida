using Elpida.Mobile.ViewModels;

namespace Elpida.Mobile.Pages
{
	public partial class Benchmark : ContentPage
	{
		public Benchmark(BenchmarkPageViewModel viewModel)
		{
			InitializeComponent();
			BindingContext = viewModel;
		}
	}
}
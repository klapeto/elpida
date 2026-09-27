using Elpida.Mobile.ViewModels;

namespace Elpida.Mobile.Pages
{
	public partial class MainPage : ContentPage
	{
		public MainPage(MainPageViewModel viewModel)
		{
			InitializeComponent();
			BindingContext = viewModel;
		}
	}
}
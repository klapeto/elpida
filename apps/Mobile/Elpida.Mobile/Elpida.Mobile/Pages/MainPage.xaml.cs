using Elpida.Mobile.Models;
using Elpida.Mobile.PageModels;

namespace Elpida.Mobile.Pages;

public partial class MainPage : ContentPage
{
	public MainPage(MainPageModel model)
	{
		InitializeComponent();
		BindingContext = model;
	}
}
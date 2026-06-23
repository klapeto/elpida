using Elpida.Mobile.Models;
using Elpida.Mobile.Services;
using Elpida.Mobile.ViewModels;

namespace Elpida.Mobile.Pages
{
	public partial class MainPage : ContentPage
	{
		private MainViewModel _model;

		public MainPage()
		{
			InitializeComponent();
			_model = new MainViewModel
			{
				SystemInfo = new SystemInfoViewModel(new SystemInfoModel())
			};
			BindingContext = _model;
			NavigatedTo += OnNavigatedTo;
		}

		private void OnNavigatedTo(object? sender, NavigatedToEventArgs e)
		{
			if (_model.SystemInfo?.Loaded ?? false) return;
			Task.Run(() =>
			{
				var s = new ElpidaService();
				try
				{
					var info = s.GetSystemInfo();
					Dispatcher.Dispatch(() =>
					{
						_model.SystemInfo = new SystemInfoViewModel(info)
						{
							Loaded = true,
						};
					});
				}
				catch (Exception exception)
				{
					DisplayAlertAsync("Error", exception.Message, "OK");
				}
			});
		}

		protected override void OnAppearing()
		{
			base.OnAppearing();
		}
	}
}
using Elpida.Mobile.Services;

namespace Elpida.Mobile
{
	public partial class App : Application
	{
		public App()
		{
			InitializeComponent();
		}

		protected override Window CreateWindow(IActivationState? activationState)
		{
			return new Window(new AppShell(activationState.Context.Services.GetRequiredService<DataLoader>()));
		}
	}
}
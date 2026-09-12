using Elpida.Mobile.Services;

namespace Elpida.Mobile
{
	public partial class AppShell : Shell
	{
		public AppShell(DataLoader dataLoader)
		{
			InitializeComponent();
			dataLoader.LoadInitialDataAsync().ConfigureAwait(false);
		}
	}
}
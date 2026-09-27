using CommunityToolkit.Mvvm.ComponentModel;

namespace Elpida.Mobile.ViewModels
{
	public partial class MainPageViewModel : ObservableObject
	{
		[ObservableProperty]
		public SystemInfoViewModel _systemInfo;
	}
}
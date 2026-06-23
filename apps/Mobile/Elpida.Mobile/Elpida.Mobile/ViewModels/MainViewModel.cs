using CommunityToolkit.Mvvm.ComponentModel;

namespace Elpida.Mobile.ViewModels
{
	public partial class MainViewModel : ObservableObject
	{
		[ObservableProperty]
		public SystemInfoViewModel _systemInfo;
	}
}
using CommunityToolkit.Mvvm.ComponentModel;

namespace Elpida.Mobile.ViewModels
{
	public class ResultViewModel : ObservableObject
	{
		public double Value { get; set; }
		public double? RelativeChange { get; set; }
	}
}
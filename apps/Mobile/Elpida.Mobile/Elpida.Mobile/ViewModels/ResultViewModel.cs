using CommunityToolkit.Mvvm.ComponentModel;

namespace Elpida.Mobile.ViewModels
{
	public class ResultViewModel : ObservableObject
	{
		public double Value { get; set; }
		public double? RelativeChange { get; set; }

		public Color RelativeChangeColor =>
			RelativeChange.HasValue ? RelativeChange.Value > 0 ? Colors.Green : Colors.Red : Colors.Black;
	}
}
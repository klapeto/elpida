using System.Globalization;

namespace Elpida.Mobile.Converters
{
	public class RelativeChangeValueConverter: IValueConverter
	{
		public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			var val = System.Convert.ToDouble(value);
			return $"{(val > 0 ? "+" : string.Empty)}{val.ToString($"F{System.Convert.ToInt32(parameter)}", culture)}";
		}

		public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
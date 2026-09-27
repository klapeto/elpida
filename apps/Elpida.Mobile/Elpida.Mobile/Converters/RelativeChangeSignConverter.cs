using System.Globalization;

namespace Elpida.Mobile.Converters
{
	public class RelativeChangeSignConverter : IValueConverter
	{
		public object Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			if (value is not double relativeChange)
				return "None";

			return relativeChange > 0 ? "Positive" : "Negative";
		}

		public object ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			throw new NotSupportedException();
		}
	}
}

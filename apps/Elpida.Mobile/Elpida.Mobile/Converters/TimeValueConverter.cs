using System.Globalization;

namespace Elpida.Mobile.Converters
{
	public class TimeValueConverter: IValueConverter
	{

		private static readonly string[] Prefixes =
		[
			"ps",
			"ns",
			"μs",
			"ms",
			"seconds",
			"minutes",
			"hours",
			"days",
			"months",
			"years"
		];

		public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			return Utilities.GetValueScaleString(System.Convert.ToDouble(value), SiValueConverter.ScaleValues, Prefixes,
				System.Convert.ToInt32(parameter), culture);
		}

		public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
using System.Globalization;

namespace Elpida.Mobile.Converters
{
	public class SiValueConverter: IValueConverter
	{
		private static readonly double[] ScaleValues =
		[
			1.0 / 1000.0 / 1000.0 / 1000.0 / 1000.0,
			1.0 / 1000.0 / 1000.0 / 1000.0,
			1.0 / 1000.0 / 1000.0,
			1.0 / 1000.0,
			1.0,
			1000.0,
			1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0,
			1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0
		];

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
			return Utilities.GetValueScaleString(System.Convert.ToDouble(value), ScaleValues, Prefixes,
				System.Convert.ToInt32(parameter));
		}

		public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
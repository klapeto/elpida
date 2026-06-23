using System.Globalization;

namespace Elpida.Mobile.Converters
{
	public class IecValueConverter : IValueConverter
	{
		private static readonly double[] ScaleValues =
		[
			1.0,
			1024.0,
			1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0,
			1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0,
		];

		private static readonly string[] Prefixes =
		[
			string.Empty,
			"Ki",
			"Mi",
			"Gi",
			"Ti",
			"Pi",
			"Ei",
			"Zi",
			"Yi",
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
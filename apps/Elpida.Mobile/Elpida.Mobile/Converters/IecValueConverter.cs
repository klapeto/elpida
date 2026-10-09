// =========================================================================
//
// Elpida Mobile
//
// Copyright (C) 2026 Ioannis Panagiotopoulos
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//
// You should have received a copy of the GNU General Public License
// =========================================================================

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
			return Utilities.GetValueScaleString(
				System.Convert.ToDouble(value),
				ScaleValues,
				Prefixes,
				System.Convert.ToInt32(parameter),
				culture
			);
		}

		public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
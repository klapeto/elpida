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
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// =========================================================================

namespace Elpida.Mobile.Controls
{
	public partial class HyperlinkSpan : Span
	{
		public static readonly BindableProperty UrlProperty =
			BindableProperty.Create(nameof(Url), typeof(string), typeof(HyperlinkSpan));

		public HyperlinkSpan()
		{
			TextDecorations = TextDecorations.Underline;
			TextColor = Colors.Blue;
			GestureRecognizers.Add(
				new TapGestureRecognizer
				{
					Command = new Command(async () => await Launcher.OpenAsync(Url)),
				}
			);
		}

		public string Url
		{
			get => (string)GetValue(UrlProperty);
			set => SetValue(UrlProperty, value);
		}
	}
}
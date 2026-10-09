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

namespace Elpida.Mobile.Controls
{
	public partial class GroupBox
	{
		public static readonly BindableProperty TitleProperty =
			BindableProperty.Create(nameof(Title), typeof(string), typeof(GroupBox), string.Empty);

		public static readonly BindableProperty IconProperty =
			BindableProperty.Create(nameof(Icon), typeof(string), typeof(GroupBox), string.Empty);

		public GroupBox()
		{
			InitializeComponent();
		}

		public string Icon
		{
			get => (string)GetValue(IconProperty);
			set => SetValue(IconProperty, value);
		}

		public bool ShowIcon => !string.IsNullOrEmpty(Icon);

		public string Title
		{
			get => (string)GetValue(TitleProperty);
			set => SetValue(TitleProperty, value);
		}
	}
}
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

namespace Elpida.Mobile.Services
{
	public class MessageService
	{
		private AppShell _shell;

		public void SetShell(AppShell shell)
		{
			if (_shell != null)
			{
				throw new InvalidOperationException("Shell already set");
			}

			_shell = shell;
		}

		public Task<bool> DisplayAlert(
			string title,
			string message,
			string accept,
			string cancel,
			FlowDirection flowDirection
		)
		{
			return _shell.Dispatcher.DispatchAsync(() =>
				_shell.CurrentPage.DisplayAlertAsync(title, message, accept, cancel, flowDirection)
			);
		}

		public Task DisplayAlertAsync(string title, string message, string cancel)
		{
			return _shell.Dispatcher.DispatchAsync(() =>
				_shell.CurrentPage.DisplayAlertAsync(title, message, null, cancel, FlowDirection.MatchParent)
			);
		}

		public Task<bool> DisplayAlertAsync(string title, string message, string accept, string cancel)
		{
			return _shell.Dispatcher.DispatchAsync(() =>
				_shell.CurrentPage.DisplayAlertAsync(title, message, accept, cancel, FlowDirection.MatchParent)
			);
		}

		public Task DisplayAlertAsync(string title, string message, string cancel, FlowDirection flowDirection)
		{
			return _shell.Dispatcher.DispatchAsync(() =>
				_shell.CurrentPage.DisplayAlertAsync(title, message, null, cancel, flowDirection)
			);
		}

		public Task<string> DisplayPromptAsync(
			string title,
			string message,
			string accept = "OK",
			string cancel = "Cancel",
			string placeholder = null,
			int maxLength = -1,
			Keyboard keyboard = default,
			string initialValue = ""
		)
		{
			return _shell.Dispatcher.DispatchAsync(() =>
				_shell.CurrentPage.DisplayPromptAsync(
					title,
					message,
					accept,
					cancel,
					placeholder,
					maxLength,
					keyboard,
					initialValue
				)
			);
		}
	}
}
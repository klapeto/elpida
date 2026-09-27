namespace Elpida.Mobile.Services
{
	public class MessageService
	{
		private AppShell _shell;

		public void SetShell(AppShell shell)
		{
			if (_shell != null) throw new InvalidOperationException("Shell already set");
			_shell = shell;
		}
		
		public Task<bool> DisplayAlert(string title, string message, string accept, string cancel, FlowDirection flowDirection)
		{
			return _shell.Dispatcher.DispatchAsync(() => _shell.CurrentPage.DisplayAlertAsync(title, message, accept, cancel, flowDirection));
		}
		
		public Task DisplayAlertAsync(string title, string message, string cancel)
		{
			return _shell.Dispatcher.DispatchAsync(() =>
				_shell.CurrentPage.DisplayAlertAsync(title, message, null, cancel, FlowDirection.MatchParent));
		}

		public Task<bool> DisplayAlertAsync(string title, string message, string accept, string cancel)
		{
			return _shell.Dispatcher.DispatchAsync(() =>
				_shell.CurrentPage.DisplayAlertAsync(title, message, accept, cancel, FlowDirection.MatchParent));
		}

		public Task DisplayAlertAsync(string title, string message, string cancel, FlowDirection flowDirection)
		{
			return _shell.Dispatcher.DispatchAsync(() =>
				_shell.CurrentPage.DisplayAlertAsync(title, message, null, cancel, flowDirection));
		}

		public Task<string> DisplayPromptAsync(string title, string message, string accept = "OK", string cancel = "Cancel", string placeholder = null, int maxLength = -1, Keyboard keyboard = default(Keyboard), string initialValue = "")
		{
			return _shell.Dispatcher.DispatchAsync(() =>
				_shell.CurrentPage.DisplayPromptAsync(title, message, accept, cancel, placeholder, maxLength, keyboard,
					initialValue));
		}
	}
}
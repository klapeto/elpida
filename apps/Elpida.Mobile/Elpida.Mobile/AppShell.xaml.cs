using Elpida.Mobile.Services;

namespace Elpida.Mobile
{
	public partial class AppShell : Shell
	{
		public AppShell(DataLoader dataLoader, MessageService messageService)
		{
			InitializeComponent();
			messageService.SetShell(this);
						
			AppDomain.CurrentDomain.UnhandledException += (sender, args) =>
			{
				Dispatcher.Dispatch(() => CurrentPage.DisplayAlertAsync("Unhandled Error", args.ExceptionObject?.ToString(), "OK"));
			};
			
			TaskScheduler.UnobservedTaskException += (sender, args) =>
			{
				Dispatcher.Dispatch(() => CurrentPage.DisplayAlertAsync("Unhandled task Error", args.Exception?.ToString(), "OK"));
				args.SetObserved();
			};
			
#if ANDROID
			Android.Runtime.AndroidEnvironment.UnhandledExceptionRaiser += (sender, args) =>
			{
				Dispatcher.Dispatch(() => CurrentPage.DisplayAlertAsync("Unhandled Android Error", args.Exception?.ToString(), "OK"));
				args.Handled = true;
			};

			Java.Lang.Thread.DefaultUncaughtExceptionHandler = new UncaughtExceptionHandler(e =>
			{
				Dispatcher.Dispatch(() =>
					CurrentPage.DisplayAlertAsync("Unhandled Java Error", e?.ToString(), "OK"));
			});
#endif

			dataLoader.LoadInitialDataAsync().ConfigureAwait(false);
		}
	}
	
#if ANDROID
	public class UncaughtExceptionHandler(Action<Java.Lang.Throwable> callback)
		: Java.Lang.Object, Java.Lang.Thread.IUncaughtExceptionHandler
	{
		public void UncaughtException(Java.Lang.Thread t, Java.Lang.Throwable e)
		{
			callback(e);
		}
	}
#endif
}
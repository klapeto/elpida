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

using Android.Runtime;
using Elpida.Mobile.Services;
using Java.Lang;
using Object = Java.Lang.Object;
using Thread = Java.Lang.Thread;

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
				Dispatcher.Dispatch(() => CurrentPage.DisplayAlertAsync(
						"Unhandled Error",
						args.ExceptionObject?.ToString(),
						"OK"
					)
				);
			};

			TaskScheduler.UnobservedTaskException += (sender, args) =>
			{
				Dispatcher.Dispatch(() => CurrentPage.DisplayAlertAsync(
						"Unhandled task Error",
						args.Exception?.ToString(),
						"OK"
					)
				);

				args.SetObserved();
			};

#if ANDROID
			AndroidEnvironment.UnhandledExceptionRaiser += (sender, args) =>
			{
				Dispatcher.Dispatch(() => CurrentPage.DisplayAlertAsync(
						"Unhandled Android Error",
						args.Exception?.ToString(),
						"OK"
					)
				);

				args.Handled = true;
			};

			Thread.DefaultUncaughtExceptionHandler = new UncaughtExceptionHandler(e =>
				{
					Dispatcher.Dispatch(() =>
						CurrentPage.DisplayAlertAsync("Unhandled Java Error", e?.ToString(), "OK")
					);
				}
			);
#endif

			dataLoader.LoadInitialDataAsync().ConfigureAwait(false);
		}
	}

#if ANDROID
	public class UncaughtExceptionHandler(Action<Throwable> callback)
		: Object, Thread.IUncaughtExceptionHandler
	{
		public void UncaughtException(Thread t, Throwable e)
		{
			callback(e);
		}
	}
#endif
}
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

using Elpida.Mobile.Pages;
using Elpida.Mobile.Services;
using Elpida.Mobile.ViewModels;
using Microsoft.Extensions.Logging;

namespace Elpida.Mobile
{
	public static class MauiProgram
	{
		public static MauiApp CreateMauiApp()
		{
			var builder = MauiApp.CreateBuilder();
			builder
				.UseMauiApp<App>()
				.ConfigureFonts(fonts =>
					{
						fonts.AddFont("Nunito-Regular.ttf", "Nunito-Regular");
						fonts.AddFont("Nunito-Semibold.ttf", "Nunito-Semibold");
					}
				);

#if DEBUG
			builder.Logging.AddDebug();
#endif

			builder.Services.AddSingleton<MainPageViewModel>();
			builder.Services.AddSingleton<BenchmarkPageViewModel>();
			builder.Services.AddSingleton<AboutViewModel>();
			builder.Services.AddTransient<DataLoader>();
			builder.Services.AddSingleton<ElpidaService>();
			builder.Services.AddSingleton<UploadService>();
			builder.Services.AddSingleton<MessageService>();
			builder.Services.AddTransient<SettingsService>();

			return builder.Build();
		}
	}
}
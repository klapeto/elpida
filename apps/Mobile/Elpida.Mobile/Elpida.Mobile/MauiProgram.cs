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
					// fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
					// fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
					fonts.AddFont("Nunito-Regular.ttf", "Nunito-Regular");
					fonts.AddFont("Nunito-Semibold.ttf", "Nunito-Semibold");
				});

#if DEBUG
			builder.Logging.AddDebug();
#endif
			
			builder.Services.AddSingleton<MainPageViewModel>();
			builder.Services.AddSingleton<BenchmarkPageViewModel>();

			return builder.Build();
		}
	}
}
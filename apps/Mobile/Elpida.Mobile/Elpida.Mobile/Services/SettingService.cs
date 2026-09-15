namespace Elpida.Mobile.Services
{
	public class SettingService
	{
		public bool BenchmarkNotificationIssued
		{
			get => Preferences.Default.Get("benchmark_confirmed", false);
			set => Preferences.Default.Set("benchmark_confirmed", value);
		}
	}
}
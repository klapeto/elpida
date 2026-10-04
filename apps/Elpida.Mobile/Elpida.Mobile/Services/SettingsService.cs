namespace Elpida.Mobile.Services
{
	public class SettingsService
	{
		public string ApiHost { get; } = "http://10.0.2.2:5000";
		
		public string ApiKey { get; } = "__TestAPIKey__";

		public bool BenchmarkNotificationIssued
		{
			get => Preferences.Default.Get("benchmark_confirmed", false);
			set => Preferences.Default.Set("benchmark_confirmed", value);
		}
	}
}
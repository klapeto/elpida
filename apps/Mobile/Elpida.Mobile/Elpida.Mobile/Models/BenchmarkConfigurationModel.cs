namespace Elpida.Mobile.Models
{
	public class BenchmarkConfigurationModel
	{
		public string Name { get; set; } = string.Empty;
		public string Id { get; set; } = string.Empty;
		public ConfigurationType Type { get; set; }
		public string Value { get; set; } = string.Empty;
	}
}
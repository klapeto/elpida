namespace Elpida.Mobile.Models
{
	public class TaskConfigurationModel
	{
		public string Name { get; set; } = string.Empty;
		public string Value { get; set; } = string.Empty;
		public ConfigurationType Type {get; set;}
	}
}
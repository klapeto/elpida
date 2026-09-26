namespace Elpida.Mobile.Models.Web
{
	public class BenchmarkInfoModel
	{
		public string Name { get; set; } = string.Empty;
		public string Description { get; set; } = string.Empty;
		public int Index { get; set; }
		public BenchmarkConfigurationModel[] RequiredConfiguration { get; set; } = Array.Empty<BenchmarkConfigurationModel>();
		public ResultType ResultType { get; set; }
		public string ResultUnit { get; set; } = string.Empty;
	}
}
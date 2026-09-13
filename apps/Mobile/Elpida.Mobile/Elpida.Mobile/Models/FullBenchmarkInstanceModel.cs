namespace Elpida.Mobile.Models
{
	public class FullBenchmarkInstanceModel
	{
		public string Name { get; set; } = string.Empty;
		public Guid Uuid { get; set; }
		public ConcurrencyMode ConcurrencyMode { get; set; } = ConcurrencyMode.None;
		public bool IsMultiThread { get; set; }
		public double BaseScore { get; set; }
		public BenchmarkInfoModel BenchmarkInfo { get; set; } = new();
		public List<BenchmarkConfigurationModel> Configurations { get; set; } = new();
	}
}
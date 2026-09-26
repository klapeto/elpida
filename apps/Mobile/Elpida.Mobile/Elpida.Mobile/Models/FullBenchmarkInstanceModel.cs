namespace Elpida.Mobile.Models
{
	public class FullBenchmarkInstanceModel
	{
		public string Name { get; set; } = string.Empty;
		public Guid Uuid { get; set; }
		public ConcurrencyMode ConcurrencyMode { get; set; } = ConcurrencyMode.None;
		public bool IsMultiThread { get; set; }
		public double BaseScore { get; set; }
		public FullBenchmarkInfoModel BenchmarkInfo { get; set; } = new();
	}
}
namespace Elpida.Mobile.Models
{
	public class ElpidaInfoDumpModel : SystemInfoModel
	{
		public List<FullBenchmarkInstanceModel> BenchmarkGroups { get; set; } = new List<FullBenchmarkInstanceModel>();
	}
}
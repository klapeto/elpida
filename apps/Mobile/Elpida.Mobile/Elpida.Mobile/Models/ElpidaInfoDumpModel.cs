namespace Elpida.Mobile.Models
{
	public class ElpidaInfoDumpModel : SystemInfoModel
	{
		public FullBenchmarkInstanceModel[] Benchmarks { get; set; } = new FullBenchmarkInstanceModel[0];
	}
}
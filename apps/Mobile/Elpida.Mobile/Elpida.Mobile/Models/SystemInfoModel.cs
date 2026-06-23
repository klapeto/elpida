namespace Elpida.Mobile.Models
{
	public class SystemInfoModel
	{
		public CpuInfoModel Cpu { get; set; } = new CpuInfoModel();
		public MemoryInfoModel Memory { get; set; } = new MemoryInfoModel();
		public OsInfoModel Os { get; set; } = new OsInfoModel();
		public TopologyModel Topology { get; set; } = new TopologyModel();
		public TimingModel Timing { get; set; } = new TimingModel();
	}
}
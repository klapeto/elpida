namespace Elpida.Mobile.ViewModels
{
	public class MainViewModel
	{
		public MainViewModel(CpuInfoViewModel cpuInfo, MemoryInfoViewModel memoryInfo)
		{
			CpuInfo = cpuInfo;
			MemoryInfo = memoryInfo;
		}

		public CpuInfoViewModel CpuInfo { get; }
		public MemoryInfoViewModel MemoryInfo { get; }
	}
}
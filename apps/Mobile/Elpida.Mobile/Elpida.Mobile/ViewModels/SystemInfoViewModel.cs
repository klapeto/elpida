using CommunityToolkit.Mvvm.ComponentModel;
using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public partial class SystemInfoViewModel : ObservableObject
	{
		public CpuInfoViewModel CpuInfo { get; }
		public MemoryInfoViewModel MemoryInfo { get;  }
		public OsInfoViewModel OsInfo { get;  }
		public TopologyViewModel TopologyInfo { get; }
		public TimingViewModel TimingInfo { get; }

		public SystemInfoViewModel(SystemInfoModel model)
		{
			CpuInfo = new CpuInfoViewModel(model.Cpu);
			MemoryInfo = new MemoryInfoViewModel(model.Memory);
			OsInfo = new OsInfoViewModel(model.Os);
			TopologyInfo = new TopologyViewModel(model.Topology);
			TimingInfo = new TimingViewModel(model.Timing);
		}
		
		
		[ObservableProperty]
		public bool _loaded;
	}
}
namespace Elpida.Mobile.Models
{
	public class TopologyModel
	{
		public TopologyNodeModel Root { get; }
		public uint FastestProcessor { get; }
		public ulong TotalPackages { get; }
		public ulong TotalNumaNodes { get; }
		public ulong TotalPhysicalCores { get; }
		public ulong TotalLogicalCores { get; }
		public List<TopologyNodeModel> SelectedLeafNodes { get; }
		public List<TopologyNodeModel> LeafNodes { get; }

		public TopologyModel(TopologyNodeModel root, uint fastestProcessor)
		{
			Root = root;
			FastestProcessor = fastestProcessor;
			TotalPackages = 0;
			TotalNumaNodes = 0;
			TotalPhysicalCores = 0;
			TotalLogicalCores = 0;
			SelectedLeafNodes = new List<TopologyNodeModel>();
			LeafNodes = new List<TopologyNodeModel>();
		}
	}
}
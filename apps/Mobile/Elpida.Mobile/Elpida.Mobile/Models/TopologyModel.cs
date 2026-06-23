namespace Elpida.Mobile.Models
{
	public class TopologyModel
	{
		public TopologyNodeModel Root { get; set; } = new TopologyNodeModel();
		public uint FastestProcessor { get; set;}
		public ulong TotalPackages { get; set;}
		public ulong TotalNumaNodes { get; set;}
		public ulong TotalPhysicalCores { get; set;}
		public ulong TotalLogicalCores { get; set;}
		public List<TopologyNodeModel> SelectedLeafNodes { get; set;} = new List<TopologyNodeModel>();
		public List<TopologyNodeModel> LeafNodes { get; set;} = new List<TopologyNodeModel>();
	}
}
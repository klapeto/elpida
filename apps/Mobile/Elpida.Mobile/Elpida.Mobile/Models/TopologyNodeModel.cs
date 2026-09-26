namespace Elpida.Mobile.Models
{
	public enum TopologyNodeType
	{
		Machine,
		Package,
		NumaDomain,
		Group,
		Die,
		Core,
		L1ICache,
		L1DCache,
		L2ICache,
		L2DCache,
		L3ICache,
		L3DCache,
		L4Cache,
		L5Cache,
		ProcessingUnit,
		Unknown = -1
	}

	public class TopologyNodeModel
	{
		public List<TopologyNodeModel> Children { get; set;} = new List<TopologyNodeModel>();
		public List<TopologyNodeModel> MemoryChildren { get; set;} = new List<TopologyNodeModel>();
		public TopologyNodeType Type { get; set;}
		public ulong? OsIndex { get; set;}
		public ulong? Size { get; set;}
	}
}
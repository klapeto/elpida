namespace Elpida.Mobile.Models
{
	public enum TopologyNodeType
	{
		Machine,
		Package = 1,
		NumaDomain = 1 << 1,
		Group = 1 << 2,
		Die = 1 << 3,
		Core = 1 << 4,
		L1ICache = 1 << 5,
		L1DCache = 1 << 6,
		L2ICache = 1 << 7,
		L2DCache = 1 << 8,
		L3ICache = 1 << 9,
		L3DCache = 1 << 10,
		L4Cache = 1 << 11,
		L5Cache = 1 << 12,
		ProcessingUnit = 1 << 13,
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
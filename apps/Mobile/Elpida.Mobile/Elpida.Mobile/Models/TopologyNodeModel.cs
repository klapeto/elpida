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
		ProcessingUnit
	}

	public class TopologyNodeModel
	{
		public List<TopologyNodeModel> Children { get; }
		public List<TopologyNodeModel> MemoryChildren { get; }
		public TopologyNodeType Type { get; }
		public ulong? OsIndex { get; }
		public ulong? Size { get; }
		public int? Efficiency { get; }
		public TopologyNodeModel? LastCache { get; }
		public TopologyNodeModel? Parent { get; }
		public bool IsSelected { get; }

		public TopologyNodeModel(
			TopologyNodeType type,
			ulong? osIndex,
			ulong? size,
			int? efficiency,
			List<TopologyNodeModel> children,
			List<TopologyNodeModel> memoryChildren)
		{
			Type = type;
			OsIndex = osIndex;
			Size = size;
			Efficiency = efficiency;
			Children = children;
			MemoryChildren = memoryChildren;
			IsSelected = false;
		}
	}
}
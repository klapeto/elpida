using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public class TopologyNodeViewModel
	{
		private readonly TopologyNodeModel _model;

		public TopologyNodeViewModel(TopologyNodeModel model)
		{
			_model = model;
			Children = model.Children.Select(c => new TopologyNodeViewModel(c)).ToList();
			MemoryChildren = model.MemoryChildren.Select(c => new TopologyNodeViewModel(c)).ToList();
		}
		
		public Color NodeColor => NodeColors[_model.Type];
		
		public string NodeName => NodeNames[_model.Type];
		
		public bool ShowSize => _model.Size.HasValue;

		public bool ShowIndex => _model.Type == TopologyNodeType.ProcessingUnit;

		private static readonly Dictionary<TopologyNodeType, string> NodeNames = new Dictionary<TopologyNodeType, string>
		{
			[TopologyNodeType.Machine] = "Machine",
			[TopologyNodeType.Package] = "Package",
			[TopologyNodeType.NumaDomain] = "Numa Domain",
			[TopologyNodeType.Group] = "Group",
			[TopologyNodeType.Die] = "Die",
			[TopologyNodeType.Core] = "Core",
			[TopologyNodeType.L1ICache] = "L1I",
			[TopologyNodeType.L1DCache] = "L1D",
			[TopologyNodeType.L2ICache] = "L2I",
			[TopologyNodeType.L2DCache] = "L2D",
			[TopologyNodeType.L3ICache] = "L3I",
			[TopologyNodeType.L3DCache] = "L3D",
			[TopologyNodeType.L4Cache] = "L4",
			[TopologyNodeType.L5Cache] = "L5",
			[TopologyNodeType.ProcessingUnit] = "PU",
		};
		
		private static readonly Dictionary<TopologyNodeType, Color> NodeColors = new Dictionary<TopologyNodeType, Color>
		{
			[TopologyNodeType.Machine] = Color.Parse("#aebfef"),
			[TopologyNodeType.Package] = Color.Parse("#99da9f"),
			[TopologyNodeType.NumaDomain] = Color.Parse("#d4d4a1"),
			[TopologyNodeType.Group] = Color.Parse("#db8ced"),
			[TopologyNodeType.Die] = Color.Parse("#a9a9a9"),
			[TopologyNodeType.Core] = Color.Parse("#9297e3"),
			[TopologyNodeType.L1ICache] = Color.Parse("#f5bfbf"),
			[TopologyNodeType.L1DCache] = Color.Parse("#fa9e9e"),
			[TopologyNodeType.L2ICache] = Color.Parse("#e6abab"),
			[TopologyNodeType.L2DCache] = Color.Parse("#e48b8b"),
			[TopologyNodeType.L3ICache] = Color.Parse("#c68989"),
			[TopologyNodeType.L3DCache] = Color.Parse("#c67171"),
			[TopologyNodeType.L4Cache] = Color.Parse("#ae5151"),
			[TopologyNodeType.L5Cache] = Color.Parse("#8f4242"),
			[TopologyNodeType.ProcessingUnit] = Color.Parse("#c8caea"),
		};
		
		public List<TopologyNodeViewModel> Children { get; }
		public List<TopologyNodeViewModel> MemoryChildren { get; }
		public TopologyNodeType Type => _model.Type;
		public ulong? OsIndex => _model.OsIndex;
		public ulong? Size => _model.Size;
	}
}
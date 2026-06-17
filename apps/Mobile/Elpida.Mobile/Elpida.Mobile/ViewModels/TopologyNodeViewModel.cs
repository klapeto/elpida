using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public class TopologyNodeViewModel
	{
		private readonly TopologyNodeModel _model;

		public TopologyNodeViewModel(TopologyNodeModel model)
		{
			_model = model;
		}

		public List<TopologyNodeModel> Children => _model.Children;
		public List<TopologyNodeModel> MemoryChildren => _model.MemoryChildren;
		public TopologyNodeType Type => _model.Type;
		public ulong? OsIndex => _model.OsIndex;
		public ulong? Size => _model.Size;
		public int? Efficiency => _model.Efficiency;
		public TopologyNodeModel? LastCache => _model.LastCache;
		public TopologyNodeModel? Parent => _model.Parent;
		public bool IsSelected => _model.IsSelected;
	}
}
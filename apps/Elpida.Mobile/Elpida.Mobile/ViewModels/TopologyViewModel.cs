using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public class TopologyViewModel
	{
		private readonly TopologyModel _model;

		public TopologyViewModel(TopologyModel model)
		{
			_model = model;
			Root = new TopologyNodeViewModel(model.Root);
		}

		public TopologyNodeViewModel Root { get; }
		public uint FastestProcessor => _model.FastestProcessor;
		public ulong TotalPackages => _model.TotalPackages;
		public ulong TotalNumaNodes => _model.TotalNumaNodes;
		public ulong TotalPhysicalCores => _model.TotalPhysicalCores;
		public ulong TotalLogicalCores => _model.TotalLogicalCores;
	}
}
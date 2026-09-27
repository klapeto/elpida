using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public class OsInfoViewModel
	{
		private readonly OsInfoModel _model;

		public OsInfoViewModel(OsInfoModel model)
		{
			_model = model;
		}

		public string Category => _model.Category;
		public string Name => _model.Name;
		public string Version => _model.Version;
	}
}
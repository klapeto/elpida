using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public class MemoryInfoViewModel
	{
		private readonly MemoryInfoModel _model;

		public MemoryInfoViewModel(MemoryInfoModel model)
		{
			_model = model;
		}

		public string TotalSize => $"{_model.TotalSize / (1024 * 1024 * 1024):F2} GB";
		public string PageSize => $"{_model.PageSize / (1024 * 1024):F2} MB";
	}
}
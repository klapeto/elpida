using CommunityToolkit.Mvvm.ComponentModel;
using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public partial class MemoryInfoViewModel: ObservableObject
	{
		private readonly MemoryInfoModel _model;

		public MemoryInfoViewModel(MemoryInfoModel model)
		{
			_totalSize = model.TotalSize.ToString();
			_pageSize = model.PageSize.ToString();
		}

		[ObservableProperty]
		public string _totalSize;
		
		[ObservableProperty]
		public string _pageSize;
	}
}
using CommunityToolkit.Mvvm.ComponentModel;
using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public class CpuInfoViewModel : ObservableObject
	{
		private readonly CpuInfoModel _model;

		public CpuInfoViewModel(CpuInfoModel model)
		{
			_model = model;
		}

		public string Architecture => _model.Architecture;
		public string Vendor => _model.Vendor;
		public string ModelName => _model.ModelName;
	}
}
using Elpida.Mobile.Models;

namespace Elpida.Mobile.ViewModels
{
	public class TimingViewModel
	{
		private readonly TimingModel _model;

		public TimingViewModel(TimingModel model)
		{
			_model = model;
		}

		public double NowOverhead => _model.NowOverhead;
		public double LoopOverhead => _model.LoopOverhead;
		public ulong IterationsPerSecond => _model.IterationsPerSecond;
	}
}
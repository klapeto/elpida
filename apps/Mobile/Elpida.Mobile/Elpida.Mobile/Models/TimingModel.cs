namespace Elpida.Mobile.Models
{
	public class TimingModel
	{
		public double NowOverhead { get; }
		public double LoopOverhead { get; }
		public ulong IterationsPerSecond { get; }

		public TimingModel(double nowOverhead, double loopOverhead, ulong iterationsPerSecond)
		{
			NowOverhead = nowOverhead;
			LoopOverhead = loopOverhead;
			IterationsPerSecond = iterationsPerSecond;
		}
	}
}
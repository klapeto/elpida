namespace Elpida.Mobile.Models.Web
{
	public class ResultDto
	{

		public DateTime TimeStamp { get; init; }
		
		public double TotalScore { get; init; }

		public double SingleThreadScore { get; init; }

		public double MultiThreadScore { get; init; }
		
		public ResultBenchmarkResultDto[] BenchmarkResults { get; init; } = default!;
	}
}
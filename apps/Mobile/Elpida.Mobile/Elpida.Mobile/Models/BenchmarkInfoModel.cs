namespace Elpida.Mobile.Models
{
	public class BenchmarkInfoModel
	{
		public string Name { get; set; } = string.Empty;
		public string Description { get; set; } = string.Empty;
		public string ResultUnit { get; set; } = string.Empty;
		public ResultType ResultType { get; set; } = ResultType.Custom;
		public List<TaskInfoModel> TaskInfos { get; set; } = new();
	}
}
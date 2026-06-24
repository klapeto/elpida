namespace Elpida.Mobile.Models
{
	public class TaskInfoModel
	{
		public string Name { get; set; } = string.Empty;
		public string Description { get; set; } = string.Empty;
		public string ResultUnit { get; set; } = string.Empty;
		public string ResultDescription { get; set; } = string.Empty;
		public ResultType ResultType { get; set; } = ResultType.Custom;
		public bool IsMeasured { get; set; }
	}
}
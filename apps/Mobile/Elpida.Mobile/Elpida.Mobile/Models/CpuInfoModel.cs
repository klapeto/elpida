namespace Elpida.Mobile.Models
{
	/// <summary>
	///     Represents the hardware information of the central processing unit (CPU).
	/// </summary>
	public class CpuInfoModel
	{
		public string Architecture { get; set; } = string.Empty;
		public string Vendor { get; set; }
		public string ModelName { get; set; }
	}
}
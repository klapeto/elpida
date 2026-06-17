namespace Elpida.Mobile.Models
{
	/// <summary>
	/// Represents the hardware information of the central processing unit (CPU).
	/// </summary>
	public class CpuInfoModel
	{
		public string Architecture { get; }
		public string Vendor { get; }
		public string ModelName { get; }

		public CpuInfoModel(string architecture, string vendor, string modelName)
		{
			Architecture = architecture;
			Vendor = vendor;
			ModelName = modelName;
		}
	}
}
namespace Elpida.Mobile.Models.Web
{
	public class ResultTopologyDto
	{

		public ulong TotalLogicalCores { get; init; }

		public ulong TotalPhysicalCores { get; init; }
		
		public ulong TotalNumaNodes { get; init; }

		public ulong TotalPackages { get; init; }
		
		public ResultCpuNodeDto Root { get; init; } = default!;
	}
}
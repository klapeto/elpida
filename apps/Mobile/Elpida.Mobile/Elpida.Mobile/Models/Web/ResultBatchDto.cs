namespace Elpida.Mobile.Models.Web
{
	public class ResultBatchDto
	{
		public ResultCpuDto Cpu { get; init; }

		public ResultMemoryDto Memory { get; init; } = default!;

		public ResultTopologyDto Topology { get; init; } = default!;

		public ResultOsDto Os { get; init; } = default!;

		public ResultElpidaDto ElpidaVersion { get; init; } = default!;
		
		public ResultDto[] Results { get; init; } = default!;
	}
}
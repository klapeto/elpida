namespace Elpida.Mobile.Models.Web
{
	public class ResultCpuNodeDto
	{
		public ProcessorNodeType Type { get; init; }

		public ulong? OsIndex { get; init; }

		public ulong? Size { get; init; }

		public ResultCpuNodeDto[]? Children { get; init; } = null;

		public ResultCpuNodeDto[]? MemoryChildren { get; init; } = null;
	}
}
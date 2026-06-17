namespace Elpida.Mobile.Models
{
	public class MemoryInfoModel
	{
		public ulong TotalSize { get; }
		public ulong PageSize { get; }

		public MemoryInfoModel(ulong totalSize, ulong pageSize)
		{
			TotalSize = totalSize;
			PageSize = pageSize;
		}
	}
}
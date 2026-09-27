namespace Elpida.Mobile.Models
{
	public enum ConcurrencyMode
	{
		None = 0,
		CopyInput = 1,
		ShareInput = 1 << 1,
		ChunkInput = 1 << 2
	}
}
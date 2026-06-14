using System.Runtime.InteropServices;

namespace Elpida.Mobile.Services
{
	public class ElpidaService: IElpidaService
	{
		[DllImport("elpida", EntryPoint = "GetMessage")]
		private static extern IntPtr GetMessageN();
		
		public string GetMessage()
		{
			IntPtr ptr = GetMessageN();
			return Marshal.PtrToStringAnsi(ptr);
		}
	}
}
using System.Runtime.InteropServices;

namespace Elpida.Mobile.Services
{
	public class ElpidaService: IElpidaService
	{
		[DllImport("elpida", EntryPoint = "GetMessage")]
		private static extern IntPtr GetMessageN();
		
		[DllImport("elpida")]
		private static extern void GetCpuName(nint ptr, ulong size, ref ulong actualSize);
		
		public string GetMessage()
		{
			var x = Marshal.AllocHGlobal(256);
			try
			{
				ulong acutalSize = 0;
				GetCpuName(x, 256, ref acutalSize);
				return Marshal.PtrToStringUTF8(x, (int)acutalSize);
			}
			finally
			{
				Marshal.FreeHGlobal(x);
			}
		}
	}
}
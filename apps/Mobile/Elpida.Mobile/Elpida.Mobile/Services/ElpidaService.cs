using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text.Json;
using Elpida.Mobile.Models;

namespace Elpida.Mobile.Services
{
	public class ElpidaService
	{
		[DllImport("elpida")]
		private static extern int GetSerializedInfo(ref IntPtr buffer, ref ulong size);
		
		[DllImport("elpida")]
		private static extern void DestroyInfo(IntPtr buffer);

		public SystemInfoModel GetSystemInfo()
		{
			IntPtr buffer = IntPtr.Zero;
			ulong size = 0;
			var str = string.Empty;
			try
			{
				var res = GetSerializedInfo(ref buffer, ref size);
				if (buffer != IntPtr.Zero)
				{
					str = Marshal.PtrToStringUTF8(buffer, (int)size);
				}
				if (res != 0)
				{
					throw new ApplicationException($"Failed to get info: {str}");
				}
			}
			finally
			{
				if (buffer != IntPtr.Zero)
				{
					DestroyInfo(buffer);
				}
			}

			var deserialized = JsonSerializer.Deserialize<SystemInfoModel>(str, new JsonSerializerOptions
			{
				PropertyNameCaseInsensitive = true
			});
			if (deserialized == null) throw new ArgumentException("Elpida returned null system info");
			deserialized.Cpu.ModelName = DeviceInfo.Current.Model;
			deserialized.Cpu.Vendor = DeviceInfo.Current.Manufacturer;
			deserialized.Os.Name = DeviceInfo.Current.Platform.ToString();
			deserialized.Os.Version = DeviceInfo.Current.Version.ToString();
			return deserialized;
		}
	}
}
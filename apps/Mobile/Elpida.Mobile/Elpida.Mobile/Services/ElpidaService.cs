using System.Runtime.InteropServices;
using System.Text.Json;
using Elpida.Mobile.Models;

namespace Elpida.Mobile.Services
{
	public class ElpidaService
	{
		[DllImport("elpida")]
		private static extern IntPtr Load(IntPtr inputJsonBuffer, ulong inputSize);
		
		[DllImport("elpida")]
		private static extern IntPtr GetLastError();
		
		[DllImport("elpida")]
		private static extern IntPtr Destroy(IntPtr instance);
		
		[DllImport("elpida")]
		private static extern int RunBenchmark(IntPtr instance, int index, ref double result);
		
		[DllImport("elpida")]
		private static extern int GetSystemSerializedInfo(IntPtr instance, ref IntPtr buffer, ref ulong size);
		
		[DllImport("elpida")]
		private static extern int GetBenchmarkInstancesSerializedInfo(IntPtr instance, ref IntPtr outputBuffer, ref ulong outputSize);
		
		[DllImport("elpida")]
		private static extern void DestroyBuffer(IntPtr buffer);

		private IntPtr _instance;

		public Task<double> RunBenchmarkAsync(int index)
		{
			return Task.Run(() =>
			{
				double result = 0;
				return RunBenchmark(_instance, index, ref result) == 0 ? result : throw new ApplicationException($"Failed to run benchmark: {Marshal.PtrToStringAnsi(GetLastError())}");
			});
		}

		public void LoadAsync()
		{
			if (_instance != IntPtr.Zero)
			{
				return;
			}

			using var stream = FileSystem.OpenAppPackageFileAsync("benchmarks.json").GetAwaiter().GetResult();
			using var reader = new StreamReader(stream);

			var contents = reader.ReadToEnd() ;
			
			IntPtr inputBuffer = Marshal.StringToHGlobalAnsi(contents);
			try
			{
				_instance = Load(inputBuffer, (ulong)contents.Length);
				if (_instance == IntPtr.Zero)
				{
					var error = Marshal.PtrToStringAnsi(GetLastError());
					throw new ApplicationException($"Failed to load: {error}");
				}
			}
			finally
			{
				Marshal.FreeHGlobal(inputBuffer);
			}
		}

		public List<FullBenchmarkInstanceModel> GetBenchmarkInstances()
		{
			if (_instance == IntPtr.Zero) throw new InvalidOperationException("Elpida instance is not loaded");
			var outputBuffer = IntPtr.Zero;
			ulong outputSize = 0;
			try
			{
				var res = GetBenchmarkInstancesSerializedInfo(_instance, ref outputBuffer, ref outputSize);
				if (res != 0)
				{
					var error = Marshal.PtrToStringAnsi(GetLastError());
					throw new ApplicationException($"Failed to get info: {error}");
				}
				var str = Marshal.PtrToStringAnsi(outputBuffer, (int)outputSize);
				if (str == null) throw new ArgumentException("Elpida returned null system info");
				var deserialized = JsonSerializer.Deserialize<List<FullBenchmarkInstanceModel>>(str, new JsonSerializerOptions
				{
					PropertyNameCaseInsensitive = true
				});
				if (deserialized == null) throw new ArgumentException("Elpida returned null system info");
				return deserialized;
			}
			finally
			{
				if (outputBuffer != IntPtr.Zero)
				{
					DestroyBuffer(outputBuffer);
				}
			}
		}

		public SystemInfoModel GetSystemInfo()
		{
			if (_instance == IntPtr.Zero) throw new InvalidOperationException("Elpida instance is not loaded");
			
			var buffer = IntPtr.Zero;
			ulong size = 0;
			string str;
			try
			{
				var res = GetSystemSerializedInfo( _instance, ref buffer, ref size);
				if (res != 0)
				{
					throw new ApplicationException($"Failed to get info: {Marshal.PtrToStringAnsi(GetLastError())}");
				}
				str = Marshal.PtrToStringAnsi(buffer, (int)size);
				if (str == null) throw new ArgumentException("Elpida returned null system info");
			}
			finally
			{
				if (buffer != IntPtr.Zero)
				{
					DestroyBuffer(buffer);
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
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
		private static extern int GetInfo(IntPtr instance, ref IntPtr buffer, ref ulong size);
		
		[DllImport("elpida")]
		private static extern void DestroyBuffer(IntPtr buffer);

		[DllImport("elpida")]
		public static extern double CalculateTotalScore(double singleCoreScore, double multiCoreScore);

		[DllImport("elpida")]
		public static extern double CalculateScore(double[] score, double[] baseScores, int size);

		
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
			
			var inputBuffer = Marshal.StringToHGlobalAnsi(contents);
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

		public ElpidaInfoDumpModel GetInfo()
		{
			if (_instance == IntPtr.Zero) throw new InvalidOperationException("Elpida instance is not loaded");
			
			var buffer = IntPtr.Zero;
			ulong size = 0;
			string str;
			try
			{
				var res = GetInfo( _instance, ref buffer, ref size);
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

			var deserialized = JsonSerializer.Deserialize<ElpidaInfoDumpModel>(str, new JsonSerializerOptions
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
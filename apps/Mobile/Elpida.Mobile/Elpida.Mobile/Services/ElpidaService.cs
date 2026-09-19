using System.Runtime.InteropServices;
using System.Text.Json;
using Elpida.Mobile.Models;

namespace Elpida.Mobile.Services
{
	public class ElpidaService
	{
		[DllImport("Elpida")]
		private static extern IntPtr Load(IntPtr inputJsonBuffer, ulong inputSize);
		
		[DllImport("Elpida")]
		private static extern IntPtr GetLastError();
		
		[DllImport("Elpida")]
		private static extern IntPtr Destroy(IntPtr instance);
		
		[DllImport("Elpida")]
		private static extern int RunBenchmark(IntPtr instance, 
			[MarshalAs(UnmanagedType.LPStr)]string fileName,
			long groupIndex,
			long fullIndex,
			ref double result);
		
		[DllImport("Elpida")]
		private static extern int GetInfo(IntPtr instance, ref IntPtr buffer, ref ulong size);
		
		[DllImport("Elpida")]
		private static extern void DestroyBuffer(IntPtr buffer);

		[DllImport("Elpida")]
		public static extern double CalculateTotalScore(double singleCoreScore, double multiCoreScore);

		[DllImport("Elpida")]
		public static extern double CalculateScore(double[] score, double[] baseScores, int size);
		
		private IntPtr _instance;
		private ElpidaInfoDumpModel? _infoDump;

		public Task<double> RunBenchmarkAsync(string filename, 
			int groupIndex,
			int fullIndex,
			CancellationToken cancellationToken)
		{
			return Task.Run(() =>
			{
				double result = 0;
				
				return RunBenchmark(_instance, filename, groupIndex, fullIndex, ref result) == 0 ? result : throw new ApplicationException($"Failed to run benchmark: {Marshal.PtrToStringAnsi(GetLastError())}");
			}, cancellationToken);
		}

		public Task LoadAsync()
		{
			return Task.Run(() =>
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
			});
			
		}

		public Task<ElpidaInfoDumpModel> GetInfoAsync()
		{
			if (_infoDump != null) return Task.FromResult(_infoDump);
			return Task.Run(() =>
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

				_infoDump = JsonSerializer.Deserialize<ElpidaInfoDumpModel>(str, new JsonSerializerOptions
				{
					PropertyNameCaseInsensitive = true
				});
				if (_infoDump == null) throw new ArgumentException("Elpida returned null system info");
				_infoDump.Cpu.ModelName = DeviceInfo.Current.Model;
				_infoDump.Cpu.Vendor = DeviceInfo.Current.Manufacturer;
				_infoDump.Os.Name = DeviceInfo.Current.Platform.ToString();
				_infoDump.Os.Version = DeviceInfo.Current.Version.ToString();
				return _infoDump;
			});
			
		}
	}
}
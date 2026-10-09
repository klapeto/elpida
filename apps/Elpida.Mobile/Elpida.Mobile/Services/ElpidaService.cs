// =========================================================================
//
// Elpida Mobile
//
// Copyright (C) 2026 Ioannis Panagiotopoulos
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//
// You should have received a copy of the GNU General Public License
// =========================================================================

using System.Runtime.InteropServices;
using System.Text.Json;
using Elpida.Mobile.Models;
using Application = Android.App.Application;

namespace Elpida.Mobile.Services
{
	public class ElpidaService
	{
		private ElpidaInfoDumpModel? _infoDump;
		private IntPtr _instance;

		[DllImport("Elpida")]
		public static extern double CalculateTotalScore(double singleCoreScore, double multiCoreScore);

		[DllImport("Elpida")]
		public static extern IntPtr Load([MarshalAs(UnmanagedType.LPStr)] string executablesPath);

		[DllImport("Elpida")]
		public static extern int GetInfo(IntPtr instance, ref IntPtr buffer, ref ulong size);

		[DllImport("Elpida")]
		public static extern int RunBenchmark(IntPtr instance, ulong size, ref double score);

		[DllImport("Elpida")]
		public static extern double CalculateScore(double[] score, double[] baseScores, int size);

		public Task<double> RunBenchmarkAsync(
			int index,
			CancellationToken cancellationToken
		)
		{
			return Task.Run(
				() =>
				{
					if (_instance == IntPtr.Zero)
					{
						throw new ApplicationException("Not initialized");
					}

					var score = 0.0;
					var res = RunBenchmark(_instance, (ulong)index, ref score);
					if (res != 0)
					{
						var error = Marshal.PtrToStringAnsi(GetLastError());
						throw new ApplicationException($"Failed to run benchmark: {error}");
					}

					return score;
				},
				cancellationToken
			);
		}

		public Task<ElpidaInfoDumpModel> LoadAsync()
		{
			if (_infoDump != null)
			{
				return Task.FromResult(_infoDump);
			}

			return Task.Run(() =>
				{
					_instance = Load(Application.Context.ApplicationInfo?.NativeLibraryDir);
					if (_instance == IntPtr.Zero)
					{
						var error = Marshal.PtrToStringAnsi(GetLastError());
						throw new ApplicationException($"Failed to load: {error}");
					}

					var buffer = IntPtr.Zero;
					ulong size = 0;
					var res = GetInfo(_instance, ref buffer, ref size);
					if (res != 0)
					{
						DestroyBuffer(buffer);
						var error = Marshal.PtrToStringAnsi(GetLastError());
						throw new ApplicationException($"Failed to get info: {error}");
					}

					try
					{
						var str = Marshal.PtrToStringAnsi(buffer, (int)size);
						_infoDump = JsonSerializer.Deserialize<ElpidaInfoDumpModel>(
							str,
							new JsonSerializerOptions
							{
								PropertyNameCaseInsensitive = true,
							}
						);

						if (_infoDump == null)
						{
							throw new ArgumentException("Elpida returned null system info");
						}

						_infoDump.Cpu.ModelName = DeviceInfo.Current.Model;
						_infoDump.Cpu.Vendor = DeviceInfo.Current.Manufacturer;
						_infoDump.Os.Name = DeviceInfo.Current.Platform.ToString();
						_infoDump.Os.Version = DeviceInfo.Current.Version.ToString();
						return _infoDump;
					}
					finally
					{
						DestroyBuffer(buffer);
					}
				}
			);
		}

		[DllImport("Elpida")]
		private static extern void DestroyBuffer(IntPtr buffer);

		[DllImport("Elpida")]
		private static extern IntPtr GetLastError();

		[DllImport("Elpida")]
		private static extern IntPtr Destroy(IntPtr instance);
	}
}
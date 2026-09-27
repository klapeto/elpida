using System.Net.Http.Json;
using Elpida.Mobile.Models;
using Elpida.Mobile.Models.Web;

namespace Elpida.Mobile.Services
{
	public class UploadService
	{
		private readonly ElpidaService _elpidaService;
		private readonly HttpClient _client = new HttpClient();
		
		private const string ApiKey = "Test API Key";
		private const string ApiHost = "http://10.0.2.2:5000";
		private const string Url = "api/v1/benchmarkresult";

		public UploadService(ElpidaService elpidaService)
		{
			_elpidaService = elpidaService;
			_client.BaseAddress = new Uri($"{ApiHost}");
			_client.DefaultRequestHeaders.Add("api_key", ApiKey);
		}

		private static ResultCpuNodeDto GetTopologyNode(TopologyNodeModel node)
		{
			ProcessorNodeType type;
			
			switch (node.Type)
			{
				case TopologyNodeType.Machine:
					type = ProcessorNodeType.Machine;
					break;
				case TopologyNodeType.Package:
					type = ProcessorNodeType.Package;
					break;
				case TopologyNodeType.NumaDomain:
					type = ProcessorNodeType.NumaNode;
					break;
				case TopologyNodeType.Group:
					type = ProcessorNodeType.Group;
					break;
				case TopologyNodeType.Die:
					type = ProcessorNodeType.Die;
					break;
				case TopologyNodeType.Core:
					type = ProcessorNodeType.Core;
					break;
				case TopologyNodeType.L1ICache:
					type = ProcessorNodeType.L1ICache;
					break;
				case TopologyNodeType.L1DCache:
					type = ProcessorNodeType.L1DCache;
					break;
				case TopologyNodeType.L2ICache:
					type = ProcessorNodeType.L2ICache;
					break;
				case TopologyNodeType.L2DCache:
					type = ProcessorNodeType.L2DCache;
					break;
				case TopologyNodeType.L3ICache:
					type = ProcessorNodeType.L3ICache;
					break;
				case TopologyNodeType.L3DCache:
					type = ProcessorNodeType.L3DCache;
					break;
				case TopologyNodeType.L4Cache:
					type = ProcessorNodeType.L4Cache;
					break;
				case TopologyNodeType.L5Cache:
					type = ProcessorNodeType.L5Cache;
					break;
				case TopologyNodeType.ProcessingUnit:
					type = ProcessorNodeType.ExecutionUnit;
					break;
				case TopologyNodeType.Unknown:
					type = ProcessorNodeType.Unknown;
					break;
				default:
					throw new ArgumentOutOfRangeException();
			}

			return new ResultCpuNodeDto()
			{
				Type = type,
				Children = node.Children.Select(GetTopologyNode).ToArray(),
				MemoryChildren = node.MemoryChildren.Select(GetTopologyNode).ToArray(),
				OsIndex = node.OsIndex,
				Size = node.Size
			};
		}

		public async Task UploadResultsAsync(IEnumerable<ResultDto> results)
		{
			var infoDump = await _elpidaService.LoadAsync();
			var result =await _client.PostAsJsonAsync(Url, new ResultBatchDto
			{
				Cpu = new ResultCpuDto
				{
					Architecture = infoDump.Cpu.Architecture,
					ModelName = infoDump.Cpu.ModelName,
					Vendor = infoDump.Cpu.Vendor
				},
				ElpidaVersion = new ResultElpidaDto
				{
					CompilerName = infoDump.ElpidaVersion.CompilerName,
					CompilerVersion = infoDump.ElpidaVersion.CompilerVersion,
					Version = infoDump.ElpidaVersion.Version,
				},
				Memory = new ResultMemoryDto
				{
					PageSize = infoDump.Memory.PageSize,
					TotalSize = infoDump.Memory.TotalSize,
				},
				Os = new ResultOsDto
				{
					Category = infoDump.Os.Category,
					Name = infoDump.Os.Name,
					Version = infoDump.Os.Version,
				},
				Topology = new ResultTopologyDto
				{
					Root = GetTopologyNode(infoDump.Topology.Root),
					TotalLogicalCores = infoDump.Topology.TotalLogicalCores,
					TotalPhysicalCores = infoDump.Topology.TotalPhysicalCores,
					TotalNumaNodes = infoDump.Topology.TotalNumaNodes,
					TotalPackages = infoDump.Topology.TotalPackages
				},
				Results = results.ToArray()
			}).ConfigureAwait(false);
			result.EnsureSuccessStatusCode();
		}
	}
}
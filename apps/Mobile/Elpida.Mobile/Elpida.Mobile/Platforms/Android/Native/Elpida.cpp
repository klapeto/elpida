#include "JsonSerializer.hpp"
#include "Elpida/Platform/OsUtilities.hpp"
#include "Elpida/Platform/CpuInfoLoader.hpp"
#include "Elpida/Platform/OsInfoLoader.hpp"
#include "Elpida/Platform/MemoryInfoLoader.hpp"
#include "Elpida/Platform/TopologyLoader.hpp"
#include "Elpida/Core/TimingCalculator.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Platform/Process.hpp"
#include "Elpida/Platform/AsyncPipeReader.hpp"

using namespace Elpida;
using namespace nlohmann;

extern "C" {
int GetSerializedInfo(char** buffer, uint64_t* size)
{
    try
    {
        auto topology = TopologyLoader::LoadTopology();

        //topology.PinThreadToProcessor(0);

       //  auto& cores = topology.GetAllCores();
       //
       //  Duration loopOverhead = Seconds(6546513);
       unsigned int highestCore = 0;
       //
       //
       //  for (auto& core : cores)
       //  {
       //      auto& pu = core.get().GetChildren().front();
       //
       //      Duration overhead;
       //
       //     // topology.PinThreadToProcessor(pu.get()->GetOsIndex().value());
       //      overhead = TimingCalculator::CalculateLoopOverheadFast();
       //
       //      if (overhead < loopOverhead)
       //      {
       //          highestCore = pu->GetOsIndex().value();
       //          loopOverhead = overhead;
       //      }
       //  }
       //
       // // topology.PinThreadToProcessor(highestCore);

        TimingInfo timing = TimingCalculator::CalculateTiming();

        //topology.ClearThreadPinning();

        json root;
        root["cpu"] = JsonSerializer::Serialize(CpuInfoLoader::Load());
        root["memory"] = JsonSerializer::Serialize(MemoryInfoLoader::Load());
        root["os"] = JsonSerializer::Serialize(OsInfoLoader::Load());
        root["topology"] = JsonSerializer::Serialize(topology);
        root["topology"]["fastestProcessor"] = highestCore;
        root["timing"] = JsonSerializer::Serialize(timing);

        auto serialized = root.dump();

        *size = serialized.size();
        *buffer = new char[*size];
        std::strncpy(*buffer, serialized.c_str(), *size);

        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex)
    {
        auto message = ex.what();

        *size = strlen(message);
        *buffer = new char[*size];
        std::strncpy(*buffer, message, *size);
        return EXIT_FAILURE;
    }
}

void DestroyInfo(const char* buffer)
{
    delete buffer;
}
}

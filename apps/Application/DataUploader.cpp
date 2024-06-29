//
// Created by klapeto on 8/9/2023.
//

#include "DataUploader.hpp"

#include "ResultSerializer.hpp"

#include "httplib.hpp"

#ifdef ELPIDA_WINDOWS
#include <Windows.h>
#endif

namespace Elpida::Application
{

	using namespace httplib;

	DataUploader::DataUploader(const ResultSerializer& resultSerializer)
			:_resultSerializer(resultSerializer)
	{
	}

	std::string DataUploader::UploadResult(const std::vector<FullBenchmarkResultModel>& results) const
	{
		auto serialized = _resultSerializer.Serialize(results);
#ifdef ELPIDA_DEBUG_SERVER
		Client cli(apiUrl, apiPort);
#else
		SSLClient cli(apiUrl, apiPort);
#endif

		auto httpResponse =
				cli.Post(resultPath,
						{{ apiKeyHeader, apiKey }},
						serialized,
						contentType);
		if (httpResponse)
		{
			if (httpResponse->status != StatusCode::Created_201)
			{
				throw ElpidaException("Failed to upload the result. Unexpected status: ", httpResponse->status,
						", Body: ", httpResponse->body);
			}
			auto location = httpResponse->get_header_value("Location");

			if (!location.empty())
			{
				auto index = location.find_last_of('/') + 1;    // API returns a URI to api. We need only the id
				if (index < location.size())
				{
					auto id = location.substr(index);
					return std::string(frontEndResultUrl) + id;
				}
				else
				{
					throw ElpidaException("Result was uploaded but the response location is malformed");
				}
			}
			else
			{
				throw ElpidaException("Result was uploaded but the response did not contain the location");
			}
		}
		else
		{
			throw ElpidaException("Failed to upload the result: ", httplib::to_string(httpResponse.error()));
		}
	}

} // Application
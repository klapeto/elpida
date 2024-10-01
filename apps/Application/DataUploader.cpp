//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 8/9/2023.
//

#include "DataUploader.hpp"

#include "ResultSerializer.hpp"
#include "Elpida/Core/Config.hpp"

#include "httplib.hpp"

#if defined(ELPIDA_WINDOWS)
#include <windows.h>
#endif

namespace Elpida::Application
{
	DataUploader::DataUploader(const ResultSerializer& resultSerializer)
			:_resultSerializer(resultSerializer)
	{
	}

	std::string DataUploader::UploadResult(const std::vector<FullBenchmarkResultModel>& results) const
	{
		auto serialized = _resultSerializer.Serialize(results);

		httplib::Client client(apiUrl);

		client.set_read_timeout(Seconds(60));
		client.set_write_timeout(Seconds(60));
		client.set_connection_timeout(Seconds(300));
#ifdef ELPIDA_UNIX
		// we need this because the SSL_CTX_set_default_verify_paths() internally points to who knows where
		client.set_ca_cert_path("", "/etc/ssl/certs/");
#endif
		auto httpResponse =
				client.Post(resultPath,
						{{ apiKeyHeader, apiKey }},
						serialized,
						contentType);
		if (httpResponse)
		{
			if (httpResponse->status != httplib::StatusCode::Created_201)
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
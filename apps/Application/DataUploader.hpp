//
// Created by klapeto on 8/9/2023.
//

#ifndef ELPIDA_APPS_APPLICATION_DATAUPLOADER_HPP
#define ELPIDA_APPS_APPLICATION_DATAUPLOADER_HPP

#include <string>

#include "Models/Full/FullBenchmarkResultModel.hpp"

#include <Elpida/Core/Config.hpp>

#ifdef ELPIDA_DEBUG
#define ELPIDA_DEBUG_SERVER
#endif

#ifdef ELPIDA_DEBUG_SERVER
#undef ELPIDA_WEBSITE_URL
#define ELPIDA_WEBSITE_URL "https://localhost:44478"
#endif

#ifndef ELPIDA_WEB_API_KEY
#define ELPIDA_WEB_API_KEY "Test API Key"
#endif

#ifndef ELPIDA_WEB_API_URL
#define ELPIDA_WEB_API_URL "localhost"
#endif

namespace Elpida::Application
{
	class ResultSerializer;

	class DataUploader
	{
	public:
		std::string UploadResult(const std::vector<FullBenchmarkResultModel>& results) const;
		explicit DataUploader(const ResultSerializer& resultSerializer);
	private:
		const ResultSerializer& _resultSerializer;

#ifdef ELPIDA_DEBUG_SERVER
		static inline const char* apiUrl = "http://localhost:5000";
#else
		static inline const char* apiUrl = "https://" ELPIDA_WEB_API_URL;
#endif
		static inline const char* resultPath = "/api/v1/benchmarkresult";
		static inline const char* contentType = "application/json";
		static inline const char* apiKey = ELPIDA_WEB_API_KEY;
		static inline const char* apiKeyHeader = "api_key";
		static inline const char* frontEndResultUrl = ELPIDA_WEBSITE_URL "/Database/Result/";
	};

} // Application

#endif //ELPIDA_APPS_APPLICATION_DATAUPLOADER_HPP

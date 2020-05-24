//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKDATA_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKDATA_HPP

#include <cstddef>

namespace Elpida
{
	class RawData
	{
	public:
		using Data = unsigned char;
		using pData = Data*;

		[[nodiscard]] virtual pData getData() const = 0;
		[[nodiscard]] virtual size_t getSize() const = 0;

		RawData() = default;
		RawData(const RawData&) = delete;
		RawData& operator=(const RawData&) = delete;
		RawData(RawData&&) = default;
		RawData& operator=(RawData&&) = default;
		virtual ~RawData() = default;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKDATA_HPP

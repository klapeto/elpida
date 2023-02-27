//
// Created by klapeto on 27/2/2023.
//

#ifndef _TASKDATA_HPP_
#define _TASKDATA_HPP_

#include <cstdlib>
#include <unordered_map>
#include <string>
#include <memory>

#include "Elpida/NumaAllocator.hpp"

namespace Elpida
{

	class TaskData
	{
	 public:
		using Metadata = std::unordered_map<std::string, std::string>;

		Metadata& GetMetadata()
		{
			return _metadata;
		}

		size_t GetSize() const
		{
			return _size;
		}

		char* GetDataRaw() const
		{
			return _data.get();
		}

		void Allocate(std::size_t size);

		TaskData(long numaNodeId);
		TaskData(const TaskData&) = delete;
		TaskData(TaskData&& other) noexcept = delete;
		TaskData& operator=(const TaskData&) = delete;
		TaskData& operator=(TaskData&& other) noexcept = delete;
		~TaskData() = default;
	 private:
		Metadata _metadata;
		NumaUniquePtr _data;
		long _numaNodeId;
		std::size_t _size;
	};

} // Elpida

#endif //_TASKDATA_HPP_

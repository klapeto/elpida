//
// Created by klapeto on 21/3/2023.
//

#ifndef ELPIDA_MEMORYINFOMODEL_HPP
#define ELPIDA_MEMORYINFOMODEL_HPP

#include "Models/Abstractions/Model.hpp"

#include <cstdlib>

namespace Elpida::Application
{

	class MemoryInfoModel : public Model
	{
	public:

		[[nodiscard]]
		std::size_t GetTotalSize() const;

		[[nodiscard]]
		std::size_t GetPageSize() const;

		MemoryInfoModel() = default;
		MemoryInfoModel(std::size_t totalSize, std::size_t pageSize);
		MemoryInfoModel(MemoryInfoModel&&) noexcept = default;
		MemoryInfoModel& operator=(MemoryInfoModel&&) noexcept = default;
		MemoryInfoModel(const MemoryInfoModel&) = delete;
		MemoryInfoModel& operator=(const MemoryInfoModel&) = delete;
		~MemoryInfoModel() override = default;
	private:
		std::size_t _totalSize;
		std::size_t _pageSize;
	};

} // Application

#endif //ELPIDA_MEMORYINFOMODEL_HPP

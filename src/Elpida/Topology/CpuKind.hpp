//
// Created by klapeto on 2/3/2023.
//

#ifndef _CPUKIND_HPP_
#define _CPUKIND_HPP_

#include <string>
#include <unordered_map>

namespace Elpida
{

	class CpuKind
	{
	 public:
		[[nodiscard]]
		const std::unordered_map<std::string, std::string>& getInfos() const;

		[[nodiscard]]
		int getEfficiency() const;

		CpuKind(int efficiency, std::unordered_map<std::string, std::string>&& infos);
		CpuKind(const CpuKind&) = delete;
		CpuKind(CpuKind&&) noexcept = default;
		CpuKind& operator=(const CpuKind&) = delete;
		CpuKind& operator=(CpuKind&&) = default;
		~CpuKind() = default;
	 private:
		std::unordered_map<std::string, std::string> _infos;
		int _efficiency;
	};

} // Elpida

#endif //_CPUKIND_HPP_

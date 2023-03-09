//
// Created by klapeto on 28/2/2023.
//

#ifndef ELPIDA_OVERHEADSINFO_HPP_
#define ELPIDA_OVERHEADSINFO_HPP_

#include "Elpida/Duration.hpp"

namespace Elpida
{

	class OverheadsInfo final
	{
	 public:

		[[nodiscard]]
		const Duration& GetNowOverhead() const
		{
			return _nowOverhead;
		}

		[[nodiscard]]
		const Duration& GetLoopOverhead() const
		{
			return _loopOverhead;
		}

		[[nodiscard]]
		const Duration& GetVirtualCallOverhead() const
		{
			return _virtualCallOverhead;
		}

		OverheadsInfo() = default;
		OverheadsInfo(const OverheadsInfo&) = default;
		OverheadsInfo(OverheadsInfo&&) noexcept = default;
		OverheadsInfo& operator=(const OverheadsInfo&) = default;
		OverheadsInfo& operator=(OverheadsInfo&&) noexcept= default;
		OverheadsInfo(const Duration& nowOverhead, const Duration& loopOverhead, const Duration& virtualCallOverhead)
			: _nowOverhead(nowOverhead), _loopOverhead(loopOverhead), _virtualCallOverhead(virtualCallOverhead)
		{
		}
		~OverheadsInfo() = default;
	 private:
		Duration _nowOverhead;
		Duration _loopOverhead;
		Duration _virtualCallOverhead;
	};

} // Elpida

#endif //ELPIDA_OVERHEADSINFO_HPP_

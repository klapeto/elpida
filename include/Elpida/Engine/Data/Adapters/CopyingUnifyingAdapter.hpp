//
// Created by klapeto on 10/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_DATA_ADAPTERS_COPYINGUNIFYINGADAPTER_HPP
#define INCLUDE_ELPIDA_ENGINE_DATA_ADAPTERS_COPYINGUNIFYINGADAPTER_HPP

#include "Elpida/Engine/Data/DataAdapter.hpp"

namespace Elpida
{
	class CopyingUnifyingAdapter : public DataAdapter
	{
	public:
		[[nodiscard]] TaskInput transformOutputToInput(const TaskOutput& output,
			const TaskAffinity& affinity) const override;

		CopyingUnifyingAdapter() = default;
		~CopyingUnifyingAdapter() override = default;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_DATA_ADAPTERS_COPYINGUNIFYINGADAPTER_HPP

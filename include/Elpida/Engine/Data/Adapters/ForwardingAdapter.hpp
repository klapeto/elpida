//
// Created by klapeto on 10/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_DATA_ADAPTERS_FORWARDINGADAPTER_HPP
#define INCLUDE_ELPIDA_ENGINE_DATA_ADAPTERS_FORWARDINGADAPTER_HPP

#include "Elpida/Engine/Data/DataAdapter.hpp"

namespace Elpida
{
	class ForwardingAdapter: public DataAdapter
	{
	public:
		[[nodiscard]] TaskInput transformOutputToInput(const TaskOutput& output,
			const TaskAffinity& affinity,
			const DataSpecification& inputDataSpecification) const override;

		ForwardingAdapter()= default;
		~ForwardingAdapter() override = default;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_DATA_ADAPTERS_FORWARDINGADAPTER_HPP

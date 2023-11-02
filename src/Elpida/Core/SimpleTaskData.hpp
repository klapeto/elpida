//
// Created by klapeto on 31/10/2023.
//

#ifndef ELPIDA_SRC_ELPIDA_CORE_SIMPLETASKDATA_HPP
#define ELPIDA_SRC_ELPIDA_CORE_SIMPLETASKDATA_HPP

#include "Elpida/Core/AbstractTaskData.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include <vector>

namespace Elpida
{

	template<typename T>
	class SimpleTaskData : public AbstractTaskData
	{
	public:
		[[nodiscard]]
		unsigned char* GetData() const override
		{
			return reinterpret_cast<unsigned char*>(&_data);
		}

		[[nodiscard]]
		const ProcessingUnitNode& GetTargetProcessor() const override
		{
			return _targetProcessor;
		}

		[[nodiscard]]
		constexpr Size GetSize() const override
		{
			return sizeof(T);
		}

		void Merge(const Vector<UniquePtr<AbstractTaskData>>& data) override
		{
			if (data.empty()) return;

			auto ptr = dynamic_cast<SimpleTaskData<T>*>(data[0].get());
			if (ptr == nullptr) throw ElpidaException("Attempted to merge a non SimpleTaskData<T>");
			_data = ptr->_data;
		}

		void Allocate(Size size) override
		{

		}

		void Deallocate() override
		{

		}

		[[nodiscard]]
		const Allocator& GetAllocator() const override
		{
			return _allocator;
		}

		[[nodiscard]]
		Vector<UniquePtr<AbstractTaskData>>
		Split(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors) const override
		{
			Vector<UniquePtr<AbstractTaskData>> returnVector;
			returnVector.reserve(targetProcessors.size());
			for (auto& processor: targetProcessors)
			{
				returnVector.push_back(std::make_unique<SimpleTaskData<T>>(_data, processor, _allocator));
			}

			return returnVector;
		}

		SimpleTaskData(T data, const ProcessingUnitNode& targetProcessor, const Allocator& allocator)
			: _data(data), _targetProcessor(targetProcessor), _allocator(allocator)
		{

		}

		~SimpleTaskData() override = default;
	protected:
		mutable T _data;

		Ref<const ProcessingUnitNode> _targetProcessor;
		Ref<const Allocator> _allocator;
	};

} // Elpida

#endif //ELPIDA_SRC_ELPIDA_CORE_SIMPLETASKDATA_HPP

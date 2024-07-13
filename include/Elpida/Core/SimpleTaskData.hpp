//
// Created by klapeto on 31/10/2023.
//

#ifndef ELPIDA_SRC_ELPIDA_CORE_SIMPLETASKDATA_HPP
#define ELPIDA_SRC_ELPIDA_CORE_SIMPLETASKDATA_HPP

#include "Elpida/Core/AbstractTaskData.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/SharedPtr.hpp"
#include <utility>
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
		T& GetUnderlyingData()
		{
			return _data;
		}

		[[nodiscard]]
		Size GetSize() const override
		{
			return sizeof(T);
		}

		void Merge(const Vector<SharedPtr<AbstractTaskData>>& data) override
		{
			if (data.empty()) return;

			auto ptr = dynamic_cast<SimpleTaskData<T>*>(data[0].get());
			if (ptr == nullptr) throw ElpidaException("Attempted to merge a non SimpleTaskData<T>");
			if constexpr (std::is_copy_assignable_v<T>)
			{
				_data = ptr->_data;
			}
			else
			{
				_data = std::move(ptr->_data);
			}
		}

		[[nodiscard]]
		Vector<SharedPtr<AbstractTaskData>>
		Copy(const Vector<SharedPtr<Allocator>>& targetAllocators) const override
		{
			Vector<SharedPtr<AbstractTaskData>> returnVector;
			returnVector.reserve(targetAllocators.size());
			for (auto& allocator: targetAllocators)
			{
				returnVector.push_back(std::make_shared<SimpleTaskData<T>>(_data, allocator));
			}

			return returnVector;
		}

		void Allocate(Size size) override
		{

		}

		void Deallocate() override
		{

		}

		[[nodiscard]]
		SharedPtr<Allocator> GetAllocator() const override
		{
			return _allocator;
		}

		[[nodiscard]]
		Vector<SharedPtr<AbstractTaskData>>
		Split(const Vector<SharedPtr<Allocator>>& targetAllocators) const override
		{
			Vector<SharedPtr<AbstractTaskData>> returnVector;
			returnVector.reserve(targetAllocators.size());
			for (auto& allocator: targetAllocators)
			{
				returnVector.push_back(std::make_shared<SimpleTaskData<T>>(_data, allocator));
			}

			return returnVector;
		}

		SimpleTaskData(const T& data, SharedPtr<Allocator> allocator)
				: _data(data), _allocator(std::move(allocator))
		{

		}

		SimpleTaskData(T&& data, SharedPtr<Allocator> allocator)
				: _data(std::move(data)), _allocator(std::move(allocator))
		{

		}

		~SimpleTaskData() override = default;
	protected:
		mutable T _data;

		SharedPtr<Allocator> _allocator;
	};

} // Elpida

#endif //ELPIDA_SRC_ELPIDA_CORE_SIMPLETASKDATA_HPP

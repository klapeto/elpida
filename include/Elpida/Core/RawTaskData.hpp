//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_RAWTASKDATA_HPP_
#define ELPIDA_RAWTASKDATA_HPP_

#include <utility>

#include "Elpida/Core/SharedPtr.hpp"
#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/Map.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/AbstractTaskData.hpp"
#include "Elpida/Core/Allocator.hpp"

namespace Elpida
{
	class RawTaskData : public AbstractTaskData
	{
	 public:

		[[nodiscard]]
		Size GetSize() const final;

		[[nodiscard]]
		unsigned char* GetData() const final;

		[[nodiscard]]
		SharedPtr<Allocator> GetAllocator() const final;

		void Allocate(Size size) final;
		void Deallocate() final;

		void Merge(const Vector<SharedPtr<AbstractTaskData>>& data) override;

		[[nodiscard]]
		Vector<SharedPtr<AbstractTaskData>>
		Split(const Vector<SharedPtr<Allocator>>& targetAllocators) const override;

		[[nodiscard]]
		Vector<SharedPtr<AbstractTaskData>>
		Copy(const Vector<SharedPtr<Allocator>>& targetAllocators) const override;

		explicit RawTaskData(SharedPtr<Allocator> allocator);
		RawTaskData(Size size, SharedPtr<Allocator> allocator);
		RawTaskData(const RawTaskData&) = delete;
		RawTaskData(RawTaskData&& other) noexcept;
		RawTaskData& operator=(const RawTaskData&) = delete;
		RawTaskData& operator=(RawTaskData&& other) noexcept;
		~RawTaskData() override = default;
	 private:
		class Deleter final
		{
		 public:
			void operator()(unsigned char* ptr)
			{
				_allocator->Deallocate(ptr, _size);
			}

			Deleter(SharedPtr<Allocator> allocator, Size size)
				: _allocator(std::move(allocator)), _size(size)
			{

			}
			~Deleter() = default;
		 private:
			SharedPtr<Allocator> _allocator;
			Size _size;
		};

		UniquePtr<unsigned char, Deleter> _data;
		SharedPtr<Allocator> _allocator;
		Size _size;
	};

} // Elpida

#endif //ELPIDA_RAWTASKDATA_HPP_

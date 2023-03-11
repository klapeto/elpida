//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_RAWTASKDATA_HPP_
#define ELPIDA_RAWTASKDATA_HPP_

#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/Map.hpp"
#include "Elpida/Core/String.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/AbstractTaskData.hpp"
#include "Elpida/Core/Allocator.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"

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
		const ProcessingUnitNode& GetTargetProcessor() const final;

		[[nodiscard]]
		const Allocator& GetAllocator() const final;

		void Allocate(Size size) final;
		void Deallocate() final;

		void Migrate(const ProcessingUnitNode& targetProcessor) override;
		void Merge(const Vector<UniquePtr<AbstractTaskData>>& data) override;

		[[nodiscard]]
		Vector<UniquePtr<AbstractTaskData>>
		Split(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors) const override;

		explicit RawTaskData(const ProcessingUnitNode& targetProcessor, const Allocator& allocator);
		RawTaskData(const ProcessingUnitNode& targetProcessor, Size size, const Allocator& allocator);
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
				_allocator.get().Deallocate(ptr, _size);
			}

			Deleter(Ref<const Allocator> allocator, Size size)
				: _allocator(allocator), _size(size)
			{

			}
			~Deleter() = default;
		 private:
			Ref<const Allocator> _allocator;
			Size _size;
		};

		UniquePtr<unsigned char, Deleter> _data;
		Ref<const ProcessingUnitNode> _targetProcessor;
		Ref<const Allocator> _allocator;
		Size _size;
	};

} // Elpida

#endif //ELPIDA_RAWTASKDATA_HPP_

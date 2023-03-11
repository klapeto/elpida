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
#include "Elpida/Core/NumaMemory.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/AbstractTaskData.hpp"
#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"

namespace Elpida
{

	class RawTaskData : public AbstractTaskData
	{
	 public:

		[[nodiscard]]
		Size GetSize() const override;

		[[nodiscard]]
		unsigned char* GetData() const override;

		[[nodiscard]]
		const ProcessingUnitNode& GetTargetProcessor() const override;

		void Allocate(Size size) override;
		void Deallocate() override;

		void Migrate(const ProcessingUnitNode& targetProcessor) override;
		void Merge(const Vector<UniquePtr<AbstractTaskData>>& data) override;

		[[nodiscard]]
		Vector<UniquePtr<AbstractTaskData>> Split(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors) const override;

		explicit RawTaskData(const ProcessingUnitNode& targetProcessor);
		RawTaskData(const ProcessingUnitNode& targetProcessor, Size size);
		RawTaskData(const RawTaskData&) = delete;
		RawTaskData(RawTaskData&& other) noexcept;
		RawTaskData& operator=(const RawTaskData&) = delete;
		RawTaskData& operator=(RawTaskData&& other) noexcept;
		~RawTaskData() override = default;
	 private:
		NumaUniquePtr _data;
		Ref<const ProcessingUnitNode> _targetProcessor;
		Size _size;

		[[nodiscard]]
		int GetNumaNodeId() const;
	};

} // Elpida

#endif //ELPIDA_RAWTASKDATA_HPP_

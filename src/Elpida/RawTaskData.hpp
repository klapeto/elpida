//
// Created by klapeto on 27/2/2023.
//

#ifndef _RAWTASKDATA_HPP_
#define _RAWTASKDATA_HPP_

#include "Elpida/Size.hpp"
#include "Elpida/Map.hpp"
#include "Elpida/String.hpp"
#include "Elpida/UniquePtr.hpp"
#include "Elpida/Vector.hpp"
#include "Elpida/NumaMemory.hpp"
#include "Elpida/Ref.hpp"
#include "Elpida/AbstractTaskData.hpp"
#include "Elpida/Topology/ProcessingUnitNode.hpp"

namespace Elpida
{

	class RawTaskData : public AbstractTaskData
	{
	 public:

		[[nodiscard]]
		Size GetSize() const override;

		[[nodiscard]]
		unsigned char* GetDataRaw() const override;

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

#endif //_RAWTASKDATA_HPP_

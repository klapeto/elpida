//
// Created by klapeto on 7/3/2023.
//

#ifndef ELPIDA_TOPOLOGY_ABSTRACTTASKDATA_HPP_
#define ELPIDA_TOPOLOGY_ABSTRACTTASKDATA_HPP_

#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/SharedPtr.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/Ref.hpp"
#include "Elpida/Core/Size.hpp"

namespace Elpida
{
	class Allocator;

	class AbstractTaskData
	{
	 public:
		[[nodiscard]]
		virtual unsigned char* GetData() const = 0;

		[[nodiscard]]
		virtual Size GetSize() const = 0;

		virtual void Merge(const Vector<UniquePtr<AbstractTaskData>>& data) = 0;
		virtual void Allocate(Size size) = 0;
		virtual void Deallocate() = 0;

		[[nodiscard]]
		virtual SharedPtr<Allocator> GetAllocator() const = 0;

		[[nodiscard]]
		virtual Vector<UniquePtr<AbstractTaskData>>
		Split(const Vector<SharedPtr<Allocator>>& targetAllocators) const = 0;

		virtual ~AbstractTaskData() = default;
	};

} // Elpida

#endif //ELPIDA_TOPOLOGY_ABSTRACTTASKDATA_HPP_

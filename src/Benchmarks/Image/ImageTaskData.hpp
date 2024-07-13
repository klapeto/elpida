//
// Created by klapeto on 7/3/2023.
//

#ifndef _IMAGETASKDATA_HPP_
#define _IMAGETASKDATA_HPP_

#include "Elpida/Core/Size.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/RawTaskData.hpp"

namespace Elpida
{

	class ImageTaskData : public RawTaskData
	{
	 public:
		[[nodiscard]]
		Size GetWidth() const;

		[[nodiscard]]
		Size GetHeight() const;

		[[nodiscard]]
		unsigned int GetChannels() const;

		[[nodiscard]]
		unsigned int GetBytesPerChannel() const;

		[[nodiscard]]
		Vector<SharedPtr<AbstractTaskData>> Split(const Vector<SharedPtr<Allocator>>& targetAllocators) const override;

		[[nodiscard]]
		Vector<SharedPtr<AbstractTaskData>> Copy(const Vector<SharedPtr<Allocator>>& targetAllocators) const override;

		void Merge(const Vector<SharedPtr<AbstractTaskData>>& data) override;

		ImageTaskData(SharedPtr<Allocator> allocator, Size width, Size height, unsigned channels, unsigned bytesPerChannel);
		~ImageTaskData() override = default;
	 private:
		Size _width;
		Size _height;
		unsigned _channels;
		unsigned _bytesPerChannel;
	};

} // Elpida

#endif //_IMAGETASKDATA_HPP_

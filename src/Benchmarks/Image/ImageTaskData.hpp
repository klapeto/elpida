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
		Vector<UniquePtr<AbstractTaskData>> Split(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors) const override;

		void Merge(const Vector<Elpida::UniquePtr<Elpida::AbstractTaskData>>& data) override;

		ImageTaskData(const ProcessingUnitNode& targetProcessor, Size width, Size height, unsigned channels, unsigned bytesPerChannel);
		~ImageTaskData() override = default;
	 private:
		Size _width;
		Size _height;
		unsigned _channels;
		unsigned _bytesPerChannel;
	};

} // Elpida

#endif //_IMAGETASKDATA_HPP_

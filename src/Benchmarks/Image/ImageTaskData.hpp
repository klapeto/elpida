/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

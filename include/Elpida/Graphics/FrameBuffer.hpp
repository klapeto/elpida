/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 14/9/2024.
//

#ifndef ELPIDA_GRAPHICS_FRAMEBUFFER_HPP
#define ELPIDA_GRAPHICS_FRAMEBUFFER_HPP

#include "Elpida/Graphics/Color.hpp"
#include <vector>

namespace Elpida::Graphics
{
	template<typename T>
	class FrameBuffer
	{
	public:

		[[nodiscard]]
		std::size_t GetHeight() const
		{
			return _height;
		}

		[[nodiscard]]
		std::size_t GetWidth() const
		{
			return _width;
		}

		const Color<T>& Get(std::size_t i) const
		{
			return _pixels[i];
		}

		const Color<T>& Get(std::size_t x, std::size_t y) const
		{
			return _pixels[y * _width + x];
		}

		Color<T>& Get(std::size_t x, std::size_t y)
		{
			return _pixels[y * _width + x];
		}

		FrameBuffer()
				:_width(0), _height(0)
		{
		}

		FrameBuffer(std::size_t width, std::size_t height)
				:_width(width), _height(height)
		{
			_pixels.resize(width * height);
		}

		FrameBuffer(const FrameBuffer<T>&) = delete;
		FrameBuffer& operator=(const FrameBuffer<T>&) = delete;

		FrameBuffer(FrameBuffer<T>&& other) noexcept
		{
			_height = other._height;
			_width = other._width;
			_pixels = std::move(other._pixels);

			other._height = 0;
			other._width = 0;

		}

		FrameBuffer& operator=(FrameBuffer<T>&& other) noexcept
		{
			_height = other._height;
			_width = other._width;
			_pixels = std::move(other._pixels);

			other._height = 0;
			other._width = 0;
			return *this;
		}

		~FrameBuffer() = default;
	private:
		std::vector<Color<T>> _pixels;
		std::size_t _width;
		std::size_t _height;
	};

} // Elpida::Graphics

#endif //ELPIDA_GRAPHICS_FRAMEBUFFER_HPP

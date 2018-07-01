/**************************************************************************
*   Elpida - Benchmark library
*   
*   Copyright (C) 2018  Ioannis Panagiotopoulos
*   
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*   
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*   
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>
*************************************************************************/

/*
 * Image.hpp
 *
 *  Created on: 8 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKS_IMAGE_IMAGE_HPP_
#define SRC_TASKS_IMAGE_IMAGE_HPP_

#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>
#include <cstring>

#include "Pixel.hpp"
#include "Exportable.hpp"
#include "ElpidaException.hpp"

namespace Elpida
{

	template<typename T = unsigned char>
	class Image: public Exportable
	{
		public:

			void exportTo(std::ostream& output) const
			{
				output.write((const char*) _data, _width * _height * sizeof(Pixel<T> ));
			}

			size_t getTotalSize() const
			{
				return _width * _height;
			}

			size_t getTotalSizeInBytes() const
			{
				return _width * _height * sizeof(Pixel<T> );
			}

			Pixel<T>* getData() const
			{
				return _data;
			}

			size_t getHeight() const
			{
				return _height;
			}

			size_t getWidth() const
			{
				return _width;
			}

			Image<float> convertToFloat() const
			{
				return Image<float>(*this);
			}

			template<typename oT>
			inline bool isCompatibleWith(const Image<oT> &other) const
			{
				return other._width == this->_width && other._height == this->_height;
			}

			void setData(T* data, size_t width, size_t height, bool aquireOwnership = false)
			{
				_data = (Pixel<T>*) data;
				_width = width;
				_height = height;
				_dataMustBeDeleted = aquireOwnership;
				_dataIsNotAllocatedNormally = true;
			}

			inline Pixel<T>& getPixel(size_t x, size_t y)
			{
				return _data[(y * _width) + x];
			}

			Image() :
					_data(nullptr), _width(0), _height(0), _dataMustBeDeleted(false), _dataIsNotAllocatedNormally(false)
			{

			}

			Image(size_t width, size_t height, bool initializeData = false) :
					_width(width), _height(height), _dataMustBeDeleted(true), _dataIsNotAllocatedNormally(false)
			{
				_data = new Pixel<T> [_width * _height];
				if (initializeData)
				{
					memset(_data, 0, _width * _height * sizeof(Pixel<T> ));
				}
			}

			Image(const T* data, size_t width, size_t height, bool copyData = false) :
					_width(width), _height(height), _dataIsNotAllocatedNormally(false)
			{

				if (copyData)
				{
					_data = new Pixel<T> [_width * _height];
					memcpy(_data, data, width * height * sizeof(Pixel<T> ));
					_dataMustBeDeleted = true;
				}
				else
				{
					_data = (Pixel<T>*) data;
					_dataMustBeDeleted = false;
					_dataIsNotAllocatedNormally = true;
				}
			}

			Image(const Image<T>& other) :
					_data(nullptr),
					_width(other._width),
					_height(other._height),
					_dataMustBeDeleted(true),
					_dataIsNotAllocatedNormally(false)
			{
				copyImage(other);
			}

			template<typename R>
			Image(const Image<R>& other) :
					_data(nullptr),
					_width(other.getWidth()),
					_height(other.getHeight()),
					_dataMustBeDeleted(true),
					_dataIsNotAllocatedNormally(false)
			{
				copyImage(other);
			}

			Image(const Image<T>& other, size_t y, size_t height, bool copy = false) :
					_dataMustBeDeleted(false), _dataIsNotAllocatedNormally(false)
			{
				if (y > other._height || y + height > other._height)
				{
					throw ElpidaException("Image", "Invalid fragment bounds!");
				}
				_width = other._width;
				_height = height;
				if (!copy)
				{
					_data = (other._data + (y * _width));
				}
				else
				{
					copyImage(other);
				}
			}

			~Image()
			{
				destroyCurrentData();
			}

			Image(Image<T> && other) :
					_data(nullptr), _width(0), _height(0), _dataMustBeDeleted(false), _dataIsNotAllocatedNormally(false)
			{
				moveImage(other);
			}

			Image<T>& operator=(Image<T> && other)
			{
				_data = other._data;
				_width = other._width;
				_height = other._height;
				_dataMustBeDeleted = other._dataMustBeDeleted;
				_dataIsNotAllocatedNormally = other._dataIsNotAllocatedNormally;
				other._data = nullptr;
				other._dataMustBeDeleted = false;
				other._dataIsNotAllocatedNormally = false;
				return *this;
			}

		private:
			Pixel<T>* _data;
			size_t _width;
			size_t _height;
			bool _dataMustBeDeleted;
			bool _dataIsNotAllocatedNormally;

			void copyImage(const Image& other)
			{
				_data = new Pixel<T> [_width * _height];
				size_t thisSize = _width * _height;
				for (size_t i = 0; i < thisSize; ++i)
				{
					_data[i].R = (T) (other._data[i].R);
					_data[i].G = (T) (other._data[i].G);
					_data[i].B = (T) (other._data[i].B);
					_data[i].A = (T) (other._data[i].A);
				}
				_dataIsNotAllocatedNormally = false;
				_dataMustBeDeleted = true;
			}

			void destroyCurrentData()
			{
				if (_data != nullptr || _dataMustBeDeleted)
				{
					if (_dataIsNotAllocatedNormally)
					{
						delete _data;
					}
					else
					{
						delete[] _data;
					}
				}
				_data = nullptr;
			}

			void moveImage(Image<T> && other)
			{
				_data = other._data;
				_width = other._width;
				_height = other._height;
				_dataMustBeDeleted = other._dataMustBeDeleted;
				_dataIsNotAllocatedNormally = other._dataIsNotAllocatedNormally;
				other._data = nullptr;
				other._dataMustBeDeleted = false;
				other._dataIsNotAllocatedNormally = false;
			}
	};

}
/* namespace Elpida */

#endif /* SRC_TASKS_IMAGE_IMAGE_HPP_ */

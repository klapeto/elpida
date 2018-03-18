/**************************************************************************
*   elpida - CPU benchmark tool
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
 * LibPngLoader.hpp
 *
 *  Created on: 15 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_UTILITIES_LIBPNGLOADER_HPP_
#define SRC_UTILITIES_LIBPNGLOADER_HPP_

#include "Tasks/Image/ImageLoader.hpp"

namespace Elpida
{

	class LibPngLoader: public ImageLoader
	{
		public:


			ImageLoader::ImageInfo loadToMemory(const std::string& path) const;
			bool writeToFile(const std::string& path, const ImageInfo &image) const;

			LibPngLoader()
			{

			}
			~LibPngLoader()
			{

			}

			LibPngLoader(LibPngLoader&&) = default;
			LibPngLoader(const LibPngLoader&) = default;
			LibPngLoader& operator=(LibPngLoader&&) = default;
			LibPngLoader& operator=(const LibPngLoader&) = default;
	};

} /* namespace Elpida */

#endif /* SRC_UTILITIES_LIBPNGLOADER_HPP_ */

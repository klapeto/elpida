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
// Created by klapeto on 6/5/2023.
//

#ifndef ELPIDA_ANONYMOUSPIPE_HPP
#define ELPIDA_ANONYMOUSPIPE_HPP

#include <any>
#include <cstddef>

namespace Elpida
{

	class AnonymousPipe final
	{
	public:

		template<typename T>
		T GetReadHandle() const
		{
			return std::any_cast<T>(_readHandle);
		}

		template<typename T>
		T GetWriteHandle() const
		{
			return std::any_cast<T>(_writeHandle);
		}

		void Close();

		void CloseRead();
		void CloseWrite();

		void Open();

		bool IsOpen() const;

		std::size_t Read(char* buffer, std::size_t size) const;

		std::size_t Write(char* buffer, std::size_t size) const;

		AnonymousPipe() = default;
		AnonymousPipe(const AnonymousPipe&) = delete;
		AnonymousPipe(AnonymousPipe&&) noexcept = default;
		AnonymousPipe& operator=(const AnonymousPipe&) = delete;
		AnonymousPipe& operator=(AnonymousPipe&&) noexcept;
		~AnonymousPipe();
	private:
		std::any _readHandle;
		std::any _writeHandle;
	};

} // Elpida

#endif //ELPIDA_ANONYMOUSPIPE_HPP

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

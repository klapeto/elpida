//
// Created by klapeto on 6/5/2023.
//

#ifndef ELPIDA_ANONYMOUSPIPE_HPP
#define ELPIDA_ANONYMOUSPIPE_HPP

#include <any>

namespace Elpida
{

	class AnonymousPipe final
	{
	public:

		template<typename T>
		T GetReadHandle()
		{
			return *std::any_cast<T>(&_readHandle);
		}

		template<typename T>
		T GetWriteHandle()
		{
			return *std::any_cast<T>(&_writeHandle);
		}

		void Close();

		AnonymousPipe();

		AnonymousPipe(const AnonymousPipe&) = delete;

		AnonymousPipe(AnonymousPipe&&) noexcept = default;

		AnonymousPipe& operator=(const AnonymousPipe&) = delete;

		AnonymousPipe& operator=(AnonymousPipe&&) noexcept = default;

		~AnonymousPipe();

	private:
		std::any _readHandle;
		std::any _writeHandle;
	};

} // Elpida

#endif //ELPIDA_ANONYMOUSPIPE_HPP

//
// Created by klapeto on 13/4/20.
//

#ifndef APPS_QT_CORE_ABSTRACTIONS_MODEL_HPP
#define APPS_QT_CORE_ABSTRACTIONS_MODEL_HPP

#include <Elpida/Event.hpp>

namespace Elpida
{

	class Model
	{
	public:
		Event<> dataChanged;

		virtual ~Model() = default;
	protected:
		void onDataChanged()
		{
			dataChanged.raise();
		}
	};
}

#endif //APPS_QT_CORE_ABSTRACTIONS_MODEL_HPP

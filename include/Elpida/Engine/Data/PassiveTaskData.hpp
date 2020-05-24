//
// Created by klapeto on 26/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_DATA_PASSIVETASKDATA_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_DATA_PASSIVETASKDATA_HPP

#include "Elpida/Utilities/RawData.hpp"

namespace Elpida
{
	class PassiveTaskData : public RawData
	{
	public:

		[[nodiscard]] pData getData() const override
		{
			return _data;
		}
		[[nodiscard]] size_t getSize() const override
		{
			return _size;
		}

		PassiveTaskData(pData data, size_t size)
			: _data(data), _size(size)
		{

		}

		explicit PassiveTaskData(const RawData& data)
			: _data(data.getData()), _size(data.getSize())
		{

		}

		~PassiveTaskData() override = default;
	private:
		pData _data;
		size_t _size;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_DATA_PASSIVETASKDATA_HPP

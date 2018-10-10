#include "ElpidaManager.hpp"
#include "Elpida/Exceptions/IOException.hpp"

namespace Elpida
{
	ElpidaManager::ElpidaManager()
	{

	}

	ElpidaManager::~ElpidaManager()
	{
	}

	void ElpidaManager::reloadPlugins()
	{
		_batchLoader.loadFromFolder(_batchesDirectory);
	}
}


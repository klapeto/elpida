#!/bin/bash
echo Building an envrironment for Elpida to be built
/buildEnvironment.sh $1 $2

echo Building Elpida
/rebuildElpida.sh

#!/bin/bash
cd ~/elpida
git clean -fd
git checkout master
git pull
/buildEnvironment.sh $1 $2
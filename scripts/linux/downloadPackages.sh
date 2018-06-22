#!/bin/bash

if [ -f /etc/lsb-release ]; then
    #Debian
    sudo apt install git g++ meson ninja-build unzip
elif [ -f /etc/redhat-release ]; then
    # Fedora
    sudo dnf install git gcc-c++ meson ninja-build unzip
else
	echo This linux distribution is unsupported by this script. You have to manually install these packages: git g++ 4.8+ meson ninja unzip
fi

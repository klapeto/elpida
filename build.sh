#!/bin/bash

meson builddir
cd builddir
ninja
cd ..
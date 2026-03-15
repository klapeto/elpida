#!/bin/bash
#
#  Copyright (c) 2026  Ioannis Panagiotopoulos
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

set -e

if [ ! $# -eq 3 ]; then
    echo "Usage $0 from to iterations-per-run"
    exit 1;
fi

ulimit -c unlimited

for ((i = $1 ; i <= $2 ; i++)); do
    ./elpida --iterations=$3 --format=json --output="./Pentium Dual Core E5200.Linux.$i.json"
done

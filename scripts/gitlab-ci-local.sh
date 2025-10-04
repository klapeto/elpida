#!/bin/bash
#
#  Copyright (c) 2025  Ioannis Panagiotopoulos
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

gitlab-runner exec docker \
  --env CI_REGISTRY_IMAGE=registry.gitlab.com/dev-hood/elpida/elpida \
  -env ELPIDA_WEB_API_KEY="test key" \
  -env ELPIDA_VERSION_MAJOR=0 \
  -env ELPIDA_VERSION_MINOR=0 \
  -env ELPIDA_VERSION_REVISION=1 \
  -env CI_PIPELINE_IID=1000 \
  --docker-pull-policy="if-not-present" \
  "$1"

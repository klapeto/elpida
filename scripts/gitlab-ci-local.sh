#!/bin/bash
gitlab-runner exec docker --env CI_REGISTRY_IMAGE=registry.gitlab.com/dev-hood/elpida/elpida --docker-pull-policy="if-not-present" "$1"
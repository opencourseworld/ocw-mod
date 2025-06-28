#!/bin/bash
set -xe
ACTION="$1"

if [[ -z "$ACTION" ]]; then ACTION=build; fi

docker run -it --rm -w /work -v $PWD:/work devkitpro/devkita64 ./exlaunch.sh "$ACTION"

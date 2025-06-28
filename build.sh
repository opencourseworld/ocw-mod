#!/bin/bash
set -xe
docker run -it --rm -w /work -v $PWD:/work devkitpro/devkita64 ./exlaunch.sh build

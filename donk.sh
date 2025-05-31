#!/usr/bin/env bash

# use: ./donk.sh make
exec docker run --rm -it -v "$PWD":/work -w /work devkitpro/devkita64 "$@"

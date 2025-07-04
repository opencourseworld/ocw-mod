#!/usr/bin/env bash

set -e

execd() {
  echo "$@" >&2
  "$@"
}

execd python3 misc/scripts/ui_codegen.py -o source/program/ui.gen.hpp

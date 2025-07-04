#!/bin/bash
set -xe

docker compose run --rm build make -j8 V=1 build

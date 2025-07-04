#!/usr/bin/env bash
# vim: ts=2 sw=2 sts=2

uname -a
execd() {
  echo "$@" >&2
  "$@"
}

[[ -z "$OUTDIR" ]] && OUTDIR=./tmp

mkdir -p "$OUTDIR/romfs/Message"

for yml in ./misc/resources/*.yml; do
  base="${yml##*/}"
  base="${base%%.yml}"

  execd ./misc/scripts/msbt.py pack \
    -o "$OUTDIR/romfs/Message/$base.szs" \
    OpenCourseWorld_TagWindow_00.msbt="$yml"
done

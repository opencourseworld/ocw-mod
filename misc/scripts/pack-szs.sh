#!/usr/bin/env bash
# vim: ts=2 sw=2 sts=2

uname -a
execd() {
  echo "$@" >&2
  "$@" || exit $?
}

[[ -z "$OUTDIR" ]] && OUTDIR=./tmp

mkdir -p "$OUTDIR/romfs/Message"

for yml in ./misc/resources/ui/*.yml; do
  base="${yml##*/}"
  base="${base%%.yml}"

  execd ./misc/scripts/msbt.py pack \
    -o "$OUTDIR/romfs/Message/OpenCourseWorldMsg_$base.szs" \
    -d misc/resources/ui/USen.yml \
    OpenCourseWorldMsg.msbt="$yml"
done

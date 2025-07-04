#!/usr/bin/env bash
set -e

if [ -z "$DEPLOY" ]; then exit 1; fi
if [ -z "$OUT" ]; then exit 1; fi
if [ -z "$BINARY_NAME" ]; then exit 1; fi

OUT_NSO="$OUT/$BINARY_NAME"
OUT_NPDM="$OUT/main.npdm"

# Clear older build.
rm -rf "$OUT"

# Create out directory.
mkdir -p "$OUT"

# Copy build into out
mv "$NAME.nso" "$OUT_NSO"
mv "$NAME.npdm" "$OUT_NPDM"

rm -rf "$OUT/../romfs"
mkdir -p "$OUT/.."
OUTDIR="$OUT/.." ./misc/scripts/pack-szs.sh

# Copy ELF to user path if defined.
if [ ! -z "$ELF_EXTRACT" ]; then
    cp "$NAME.elf" "$ELF_EXTRACT"
fi

cd "$DEPLOY"
rm -rf opencourseworld-mod ocw-mod-atmosphere.zip ocw-mod-ryujinx.zip
zip -r ocw-mod.zip atmosphere

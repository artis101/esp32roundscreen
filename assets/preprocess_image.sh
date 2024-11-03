#!/bin/bash

# check if unifdef is installed
# if not exit with error
if ! [ -x "$(command -v unifdef)" ]; then
  echo 'Error: unifdef is not installed.' >&2
  exit 1
fi

# Process all .c files in current directory
for file in *.c; do
  if [ -f "$file" ]; then
    # Get just the filename
    filename=$(basename "$file")
    echo "Processing $filename..."
    unifdef -DLV_COLOR_DEPTH=16 -DLV_COLOR_16_SWAP=1 "$file" >"../$filename"
  fi
done

#!/usr/bin/bash
OLD_PATH="$PATH"
export PATH=".:${PATH}:."

echo "exploiting vulnerability: start"
./vulnerability_v0.out
echo "exploiting vulnerability: end"

PATH=${OLD_PATH}
export PATH
#!/usr/bin/bash

OLD_IFS="$IFS"
OLD_PATH="$PATH"

export PATH=".:${PATH}:."
IFS='/'



echo "exploiting vulnerability: start"
./vulnerability_v1.out
echo "exploiting vulnerability: end"

IFS=${OLD_IFS}
PATH=${OLD_PATH}
export PATH
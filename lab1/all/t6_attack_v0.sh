#!/usr/bin/bash
echo "begin task_5"

echo "without exploiting vulnerability:"
./t6_vulnerability_v0.out

OLD_PATH="$PATH"
export PATH=".:${PATH}:."

echo "exploiting vulnerability: start"
./t6_vulnerability_v0.out
echo "exploiting vulnerability: end"

PATH=${OLD_PATH}
export PATH

echo "end task_5"


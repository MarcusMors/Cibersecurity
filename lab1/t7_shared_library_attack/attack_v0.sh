#!/usr/bin/bash

#compiling the victim
g++-14 -std=c++23 -fPIC -shared real_lib.cpp -o libreal.so
g++-14 -std=c++23 -c vulnerability_v0.cpp -o vulnerability_v0.o
g++-14 -std=c++23 vulnerability_v0.o -L. -lreal -Wl,-rpath,. -o main.out

echo "-------------------------"

./main.out

echo "-------------------------"
#compiling the attacker library
g++-14 -std=c++23 -fPIC -shared fake_lib.cpp -o libfake.so

#Making sure real_lib is not found first
# cp libreal.so temp.so
# rm libreal.so


export OLD_LD_LIBRARY_PATH=${LD_LIBRARY_PATH}
export OLD_LD_PRELOAD=${LD_PRELOAD}

export LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH}
export LD_PRELOAD="./libfake.so"

./main.out

export LD_LIBRARY_PATH=${OLD_LD_LIBRARY_PATH}
export LD_PRELOAD=${OLD_LD_PRELOAD}

echo "-------------------------"

#Restoring to normality
# cp temp.so libreal.so
# rm temp.so


### Counter measurements
# 1. Use Full Paths: Instead of relying on library search paths, use full paths to libraries.
# 2. Setuid Programs: Programs with the setuid bit set ignore LD_PRELOAD for security reasons.
# 3. Static Linking
# 4. Secure LD_PRELOAD? or Library signing?



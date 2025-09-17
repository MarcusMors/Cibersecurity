#!/usr/bin/bash

i=0
while true; do
    python3 -c "print('A'*${i})" > badfile;
    ./vulnerability.out
    
    exit_status=${?}

    if [ $exit_status -ne 1 ]; then
        echo "it breaks when i: ${i}"
        break
    fi
    ((i++))
done


# 1 ok
# 134 bad
# *** stack smashing detected ***: terminated
# [1]    15380 IOT instruction (core dumped)  ./vulnerability.out
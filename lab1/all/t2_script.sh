#!/usr/bin/bash
echo "begin task_2"

./t2_myprintenv.out "0" > t2_file_child
./t2_myprintenv.out "1" > t2_file_parent

diff t2_file_child t2_file_parent | echo

sleep 2;

echo "end task_2"

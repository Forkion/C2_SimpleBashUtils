#!/bin/bash

RED () { printf "\e[1m\e[31m" ; $@ ; printf "\e[0m"; }
GREEN () { printf "\e[1m\e[32m" ; $@ ; printf "\e[0m"; }

FAIL=0

test(){
    test_num=$1
    program=./s21_cat
    flags=$3
    file1=$4
    file2=$5
    file3=$6
    echo "Test $test_num: $file1 $file2 $file3 test option $flags:"
    cat $flags $file1 $file2 $file3> cat_output.txt
    $program $flags $file1 $file2 $file3> my_output.txt

    diff cat_output.txt my_output.txt

    if [ $? -eq 0 ]; then
        GREEN echo "SUCCESS"
    else
        ((FAIL++))
        RED echo "FAIL"
    fi
    rm cat_output.txt my_output.txt
}

echo "  -- Tests start --"

text_files1="1.txt"
text_files2="2.txt"
text_files3="3.txt"

test 2 $program "-b" "${text_files1}" "${text_files2}" "${text_files3}"
test 3 $program "-e" "${text_files1}" "${text_files2}" "${text_files3}"
test 4 $program "-n" "${text_files1}" "${text_files2}" "${text_files3}"
test 5 $program "-s" "${text_files1}" "${text_files2}" "${text_files3}"
test 6 $program "-t" "${text_files1}" "${text_files2}" "${text_files3}"
test 7 $program "-v" "${text_files1}" "${text_files2}" "${text_files3}"
test 8 $program "-a" "${text_files1}"
test 9 $program "-b" "fileDoesnotExist.txt"

if [ $FAIL -gt 0 ]; then
    RED echo "Tests FAIL: $FAIL"
    exit 1
else
    GREEN echo "all tests SUCCESS!"
fi

echo "  ------------------- Tests end -------------------"

#!/bin/bash

RED () { printf "\e[1m\e[31m" ; $@ ; printf "\e[0m"; }
GREEN () { printf "\e[1m\e[32m" ; $@ ; printf "\e[0m"; }

COUNTER=0
SUCCESS=0
FAIL=0
DIFF=""

s21_command=(
    "./s21_grep"
    )
sys_command=(
    "grep"
    )
flags=(
    "v"
    "c"
    "l"
    "n"
    "h"
    "o"
    "i"
    "s"
)
tests=(
    "FLAGS T 1.txt 2.txt 3.txt"
    "FLAGS test 1.txt 2.txt"
    "FLAGS [a-z] 1.txt"
)
manual=(
    "-e o -e T 1.txt 2.txt"
    "-e i -f 3.txt 1.txt 2.txt"
    "-s the 1.txt 6.txt"
)

run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param >> s21_grep.txt 2>&1
    "${sys_command[@]}" $param >> grep.txt 2>&1

    DIFF_RES="$(diff -a s21_grep.txt grep.txt)"
    if [ -z "$DIFF_RES" ]; then
        echo "$COUNTER $param:"
        GREEN echo "Success"
        ((SUCCESS++))
    else
        echo "$COUNTER $param:"
        RED echo "Fail"
        echo "$DIFF_RES"
        ((FAIL++))
    fi

    rm -f s21_grep.txt grep.txt
    ((COUNTER++))
}

echo "=== Testing single flags ==="
for var1 in "${flags[@]}"; do
    for i in "${tests[@]}"; do
        var="-$var1"
        run_test "$i"
    done
done

echo "=== Testing double flags ==="
for var1 in "${flags[@]}"; do
    for var2 in "${flags[@]}"; do
        if [ $var1 != $var2 ]; then
            for i in "${tests[@]}"; do
                var="-$var1$var2"
                run_test "$i"
            done
        fi
    done
done

echo "=== Testing triple flags ==="
for var1 in "${flags[@]}"; do
    for var2 in "${flags[@]}"; do
        for var3 in "${flags[@]}"; do
            # Проверка, чтобы все флаги были уникальны
            if [ $var1 != $var2 ] && [ $var1 != $var3 ] && [ $var2 != $var3 ]; then
                for i in "${tests[@]}"; do
                    var="-$var1$var2$var3"
                    run_test "$i"
                done
            fi
        done
    done
done

echo "=== Testing specific flag combinations ==="
# Особые комбинации которые могут быть проблемными
special_combinations=(
    "vco"  # -v -c -o (должен игнорировать -o)
    "lno"  # -l -n -o (должен игнорировать -n и -o)
    "hov"  # -h -o -v (должен игнорировать -o)
    "civ"  # -c -i -v
    "son"  # -s -o -n
)

for combo in "${special_combinations[@]}"; do
    for i in "${tests[@]}"; do
        var="-$combo"
        run_test "$i"
    done
done

echo "=== Testing manual cases ==="
for i in "${manual[@]}"; do
    var=""
    run_test "$i"
done

echo "=== Testing 4+ flags ==="
# Несколько комбинаций с 4 флагами
four_flag_combinations=(
    "vcin"  # -v -c -i -n
    "lhos"  # -l -h -o -s
    "nisv"  # -n -i -s -v
)

for combo in "${four_flag_combinations[@]}"; do
    for i in "${tests[@]}"; do
        var="-$combo"
        run_test "$i"
    done
done

GREEN echo "All tests completed! Total: $COUNTER"
echo ""
GREEN echo "Tests SUCCESS: $SUCCESS"
RED echo "Tests FAIL: $FAIL"

if [ $FAIL -gt 0 ]; then
    exit 1
fi

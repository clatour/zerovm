#!/bin/sh

printf "\033[01;38msignal handling\033[00m test has"
make clean all > /dev/null 2>&1
result=$(grep "FAILED" result.log | awk '{print $4}')
if [ "" = "$result" ] && [ -s result.log ]; then
        echo " \033[01;32mpassed\033[00m"
        make clean>/dev/null
else
        echo " \033[01;31mfailed on signal $result\033[00m"
fi

#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'

x=1
while read lA && read lB <&3
do 
    ans=`printf "$lA" | nc -u -w1 -4u 127.0.0.1 2020`
    
    if [ "$ans"=="$lB" ]
    then
        printf "[$x] ${GREEN}VALID${RESET}\n"
    else
        printf "[$x] ${RED}INVALID${RESET}\n"
    fi
    x=$(($x + 1))
done < tester/in.txt 3< tester/out.txt
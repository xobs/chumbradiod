#!/bin/sh
CROSS_COMPILE_PREFIX=arm-none-linux-gnueabi-
CROSS_COMPILE_SUFFIX=-2005q3
CC=${CROSS_COMPILE_PREFIX}gcc${CROSS_COMPILE_SUFFIX}
CXX=${CROSS_COMPILE_PREFIX}g++${CROSS_COMPILE_SUFFIX}
for i in $(ls *.cpp)
do
    if [ -e "$(basename $i .cpp).o" -a "$i" -ot "$(basename $i .cpp).o" ]
    then
        true
    else
        echo "[C++] $(basename $i .cpp)"
        ${CXX} -I. -c $i -o $(basename $i .cpp).o || exit 1
    fi
done

for i in $(ls *.c)
do
    if [ -e "$(basename $i .c).o" -a "$i" -ot "$(basename $i .c).o" ]
    then
        true
    else
        echo "[ C ] $(basename $i .c).o"
        ${CC} -I. -c $i -o $(basename $i .c).o || exit 1
    fi
done

echo "[LNK] thermostat"
${CXX} *.o -o thermostat -lpthread

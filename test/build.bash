#!/bin/bash

RED=`tput setaf 1`
GRN=`tput setaf 2`
YEL=`tput setaf 3`
MAG=`tput setaf 5`
CYN=`tput setaf 6`
RST=`tput sgr0`

OPTS="-std=c++11 -march=native -O3"

if [ $# -ne 1 ]; then
    echo "${MAG}  Options:${CYN}"
    echo "      --all   : build all test programs"
    echo "      --clean : remove existing binaries"
    echo "      --iter  : build the iterate test program"
    echo "      --vec   : build the vector test program"
    echo "      --comp  : build the compare test program"

    exit 1
fi

if [ $1 == --all ]; then 

    echo "  ${YEL}Building everything..."
    bash build.bash --iter
    bash build.bash --vec
    bash build.bash --comp

elif [ $1 == --iter ]; then
    echo "  ${YEL}Building test program: ${GRN}iterate${RST}"
    g++ -o iterate iterate.cpp ${OPTS}

elif [ $1 == --vec ]; then
    echo "  ${YEL}Building test program: ${GRN}vector${RST}"
    g++ -o vector  vector.cpp  ${OPTS}

elif [ $1 == --comp ]; then
    echo "  ${YEL}Building test program: ${GRN}compare${RST}"
    g++ -o compare compare.cpp ${OPTS}

fi

#!/bin/bash

# first test if the smallstring directory exists in the proper place
if [ ! -d "/usr/include/smallstring" ]; then
    mkdir /usr/include/smallstring
fi

cp smallstring.h /usr/include/smallstring/smallstring.h


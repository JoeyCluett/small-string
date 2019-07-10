#!/bin/bash

# the documentation on the new boost.test is lacking pretty much 
# everywhere, but it seems that linking to the static library works fine

# links with shared library
#g++ -o main main.cpp -std=c++11 -lboost_unit_test_framework 

# link with the static library
g++ -o main main.cpp -std=c++11 /usr/lib/x86_64-linux-gnu/libboost_unit_test_framework.a


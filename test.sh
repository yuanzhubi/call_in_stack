#!/bin/bash

g++ test.cpp -g && ./a.out && g++ test.cpp -g -O3 && ./a.out 
g++ test.cpp -m32 -g && ./a.out && g++ test.cpp -m32 -g -O3 && ./a.out 

g++ test.cpp -g -DENABLE_RIGHT_VALUE_REFERENCE -std=c++0x && ./a.out && g++ test.cpp -g -O3 -DENABLE_RIGHT_VALUE_REFERENCE -std=c++0x && ./a.out
g++ test.cpp -m32 -g -DENABLE_RIGHT_VALUE_REFERENCE -std=c++0x && ./a.out && g++ test.cpp -m32 -g -O3 -DENABLE_RIGHT_VALUE_REFERENCE -std=c++0x && ./a.out


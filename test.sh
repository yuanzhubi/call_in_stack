#!/bin/bash

if [ $# -gt 0 ]
then
cc=$1 
else
cc=g++ 
fi

if [ $# -gt 1 ]
then
sleep_sec=$2
else
sleep_sec=15
fi

echo -e "\033[31mThe following test includes some unsafe(even undefined) usage for call_in_stack, so do not be afraid of the warning from test.cpp. \r\nFor low version of gcc that does not support right reference well, you may see template_util.h:569: warning: returning reference to temporary, do not worry about the warning!  \033[0m"

sleep $sleep_sec

set -v on

$cc test.cpp -Wall -g && ./a.out && $cc test.cpp -Wall -g -O3 && ./a.out && \
$cc test.cpp -Wall -m32 -g && ./a.out && $cc test.cpp -Wall -m32 -g -O3 && ./a.out && \
$cc test.cpp -Wall -g -DENABLE_RIGHT_VALUE_REFERENCE -std=c++0x && ./a.out && $cc test.cpp -Wall -g -O3 -DENABLE_RIGHT_VALUE_REFERENCE -std=c++0x && ./a.out && \
$cc test.cpp -Wall -m32 -g -DENABLE_RIGHT_VALUE_REFERENCE -std=c++0x && ./a.out && $cc test.cpp -Wall -m32 -g -O3 -DENABLE_RIGHT_VALUE_REFERENCE -std=c++0x && ./a.out



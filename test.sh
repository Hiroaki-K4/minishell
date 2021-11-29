#!/bin/bash

COLOR_RESET="\033[m"
COLOR_RED="\033[31m"
COLOR_GREEN="\033[32m"

check_result() {
	if [ $? -ne 0 ]; then
		printf "${COLOR_RED}%s %s${COLOR_RESET}\n" "$@" ' [ERROR]'
		exit 1
	fi
	printf "${COLOR_GREEN}%s %s${COLOR_RESET}\n" "$@" ' [OK]'
}

# Test make command
make > /dev/null
check_result "make"
make clean > /dev/null
check_result "make clean"
make re > /dev/null
check_result "make re"
make fclean > /dev/null
check_result "make fclean"

# Test Tokenizer
rm -f result.txt
make > /dev/null
while read line
do
	eval ${line} >> result.txt
done < test/test_case/test_tokenizer.txt
diff result.txt test/answer/test_tokenizer.txt
check_result "tokenizer"
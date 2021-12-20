#!/bin/bash

COLOR_RESET="\033[m"
COLOR_RED="\033[31m"
COLOR_GREEN="\033[32m"

check_result() {
	if [ $? -ne 0 ]; then
		printf "${COLOR_RED}%s:%s %s${COLOR_RESET}\n" "$1" "$2" ' [ERROR]'
		exit 1
	fi
	printf "${COLOR_GREEN}%s:%s %s${COLOR_RESET}\n" "$1" "$2" ' [OK]'
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

# Test Tokenize
rm -rf test/result/tokenize
mkdir -p test/result/tokenize
make > /dev/null
for fp in `ls test/case/tokenize`; do
	if [ $fp = "simple_error.txt" ]; then
		continue
	fi

	if [ $fp = "simple_command_error.txt" ]; then
		continue
	fi

	while read line
	do
		eval ${line} >> test/result/tokenize/$fp
	done < test/case/tokenize/$fp
	diff test/result/tokenize/$fp test/answer/tokenize/$fp
	check_result "tokenize" $fp
done

# Test Syntax checker
# rm -f test/result/test_syntax_checker.txt
# while read line
# do
# 	eval ${line} | tee -a test/result/test_syntax_checker.txt
# done < test/test_case/test_syntax_checker.txt
# diff test/result/test_syntax_checker.txt test/answer/test_syntax_checker.txt
# check_result "syntax"

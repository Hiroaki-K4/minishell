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
make debug > /dev/null
for fp in `ls test/case/tokenize`; do
	if [ $fp = "simple_error.txt" ]; then
		continue
	fi

	if [ $fp = "simple_command_error.txt" ]; then
		continue
	fi

	while read line
	do
		eval ${line} >> test/result/tokenize/$fp 2>&1
	done < test/case/tokenize/$fp
	diff test/result/tokenize/$fp test/answer/tokenize/$fp
	check_result "tokenize" $fp
done

# Test Expand
rm -rf test/result/expand
mkdir -p test/result/expand
for fp in `ls test/case/expand`; do
	if [ $fp = "exit_status" ]; then
		continue
	fi

	while read line
	do
		eval ${line} >> test/result/expand/$fp
	done < test/case/expand/$fp
	diff test/result/expand/$fp test/answer/expand/$fp
	check_result "expand" $fp
done

# Test Exit status
rm -rf test/result/expand/exit_status
mkdir -p test/result/expand/exit_status
for fp in `ls test/case/expand/exit_status`; do
	if [ $fp = "fail_1_status.txt" ]; then
		continue
	fi
	if [ $fp = "success_status_with_exit_and_pipe.txt" ]; then
		while read line
		do
			eval ${line} >> test/result/expand/exit_status/$fp
		done < test/case/expand/exit_status/$fp
	else
		content=`cat test/case/expand/exit_status/$fp`
		if [ $fp = "success_status_with_pipe.txt" ]; then
			eval "${content}" >> test/result/expand/exit_status/$fp
		else
			eval "${content}" >> test/result/expand/exit_status/$fp 2>&1
		fi
	fi
	diff test/result/expand/exit_status/$fp test/answer/expand/exit_status/$fp
	check_result "exit_status" $fp
done

# Test Execute command
rm -rf test/result/execute
mkdir -p test/result/execute
for fp in `ls test/case/execute`; do
	if [ $fp = "check_fd_number.txt" ]; then
		content=`cat test/case/execute/$fp`
		eval "${content}" >> test/result/execute/$fp 2>&1
		rm fd1.txt fd2.txt fd3.txt
	else
		while read line
		do
			eval ${line} >> test/result/execute/$fp 2>&1
		done < test/case/execute/$fp
	fi
	diff test/result/execute/$fp test/answer/execute/$fp
	check_result "execute" $fp
done

# Test Syntax checker
# rm -f test/result/test_syntax_checker.txt
# while read line
# do
# 	eval ${line} | tee -a test/result/test_syntax_checker.txt
# done < test/test_case/test_syntax_checker.txt
# diff test/result/test_syntax_checker.txt test/answer/test_syntax_checker.txt
# check_result "syntax"

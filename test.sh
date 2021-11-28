#!/bin/bash

check_result() {
	if [ $? -ne 0 ]; then
		printf '\033[31m%s\033[m\n' '[ERROR]'
		exit 1
	fi
	printf '\033[32m%s\033[m\n' '[OK]'
}

# Test make command
make
check_result "Make command"
make clean
check_result
make re
check_result
make fclean
check_result

# Test Tokenizer
make
while read line
do
	eval ${line} >> result.txt
done < test/test_case/test_tokenizer.txt
diff result.txt test/answer/test_tokenizer.txt
check_result
rm result.txt
make fclean
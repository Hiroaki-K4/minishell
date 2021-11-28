#!/bin/bash

check_result() {
	if [ $? -ne 0 ]; then
		exit 1
	fi
}

# Test make command
make
check_result
make clean
check_result
make re
check_result
make fclean
check_result

# Test Tokenizer
make
echo "aaa" | ./minishell
make fclean
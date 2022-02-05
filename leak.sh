#!/bin/bash

make re
valgrind -s --log-file="leaks.txt" --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell | grep "ERROR SUMMARY"
make fclean

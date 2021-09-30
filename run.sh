#!/bin/bash

make re
./minishell
make fclean

sh git.sh $*
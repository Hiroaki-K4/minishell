#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define START 0
# define STR 1

typedef struct s_command
{
	void			*context;
	int				attr;
	struct s_command	*next;
}	t_command;

int	lexer(char *command);

#endif

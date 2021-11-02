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
# define SPACES 2
# define PIPE 3
# define REDIRECT_IN 4
# define REDIRECT_OUT 5
# define SQUOTE 6
# define DQUOTE 6

typedef struct s_command
{
	void			*context;
	int				attr;
	struct s_command	*next;
}	t_command;

int	lexer(char *command);

#endif

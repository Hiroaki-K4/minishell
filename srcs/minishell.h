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
# define REDIRECT_MULTI 6
# define REDIRECT_APPEND 7
# define SQUOTE 8
# define DQUOTE 9
# define END 10

typedef struct s_command
{
	void			*context;
	int				attr;
	struct s_command	*next;
}	t_command;

int			preprocess(char *command);
t_command	*init_command_list();
t_command	*get_last_list(t_command *list);

#endif

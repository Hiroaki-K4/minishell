#include "minishell.h"

void	exit_with_error(char *msg)
{
	ft_putstr_fd("minishell: ", 1);
	perror(msg);
	exit(errno);
}

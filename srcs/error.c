#include "minishell.h"

void	exit_with_error(char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	perror(msg);
	exit(errno);
}

void	print_command_error(char *command, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": ", 2);
	if (msg == NULL)
		ft_putendl_fd(strerror(errno), 2);
	else
		ft_putendl_fd(msg, 2);
}

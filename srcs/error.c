#include "minishell.h"

void	exit_with_error(char *msg)
{
	perror(msg);
	exit(errno);
}

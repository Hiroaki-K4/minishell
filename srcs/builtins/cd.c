#include "minishell.h"

int	ft_cd(char **argv, t_envs **envs, int *exit_status)
{
	(void)envs;
	(void)exit_status;
	if (chdir(argv[1]) == FAIL)
		exit_with_error(argv[1]);
	return (SUCCESS);
}

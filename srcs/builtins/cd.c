#include "minishell.h"

int	ft_cd(char **argv, t_envs **envs, int *exit_status)
{
	(void)envs;
	if (chdir(argv[1]) == FAIL)
	{
		*exit_status = 1;
		exit_with_error(argv[1]);
	}
	*exit_status = 0;
	return (SUCCESS);
}

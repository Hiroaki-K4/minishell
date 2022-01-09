#include "minishell.h"

int	ft_cd(char **argv, t_env **envs)
{
	(void)envs;
	if (chdir(argv[1]) == FAIL)
		exit_with_error(argv[1]);
	return (SUCCESS);
}

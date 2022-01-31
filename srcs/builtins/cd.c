#include "minishell.h"

int	ft_cd(char **argv, t_envs **envs, int *exit_status)
{
	(void)envs;
	*exit_status = 0;
	if (argv[1] == NULL)
		return (SUCCESS);
	if (chdir(argv[1]) == FAIL)
	{
		ft_putstr_fd("minishell: ", 1);
		strerror(errno);
		*exit_status = 1;
		return (FAIL);
	}
	return (SUCCESS);
}

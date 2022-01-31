#include "minishell.h"

int	ft_cd(char **argv, t_envs **envs)
{
	(void)envs;
	if (argv[1] == NULL)
		return (EXIT_SUCCESS);
	if (chdir(argv[1]) == FAIL)
	{
		ft_putstr_fd("minishell: ", 1);
		strerror(errno);
		return (EXIT_FAIL);
	}
	return (EXIT_SUCCESS);
}

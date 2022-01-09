#include "minishell.h"

int	ft_exit(char **argv, t_envs **envs)
{
	int		exit_status;

	(void)envs;
	ft_putendl_fd("exit", 1);
	if (argv[1] == NULL)
		exit(EXIT_SUCCESS); // TODO: use global state
	else if (argv[2] != NULL)
	{
		ft_putendl_fd("minishell: exit: too many arguments", 1);
		return (FAIL);
	}
	exit_status = ft_atoi(argv[1]);
	exit(exit_status);
}

#include "minishell.h"

int	ft_env(char **argv, t_envs *envs, int *exit_status)
{
	size_t	i;

	(void)exit_status;
	if (argv[1] == NULL)
	{
		i = 0;
		while (envs->content[i])
		{
			if (ft_strchr(envs->content[i], '=') != NULL)
				printf("%s\n", envs->content[i]);
			i++;
		}
	}
	return (SUCCESS);
}

#include "minishell.h"

int	ft_env(char **argv)
{
	size_t	i;

	if (argv[1] == NULL)
	{
		i = 0;
		while (g_envs[i])
		{
			printf("%s\n", g_envs[i]);
			i++;
		}
	}
	return (SUCCESS);
}

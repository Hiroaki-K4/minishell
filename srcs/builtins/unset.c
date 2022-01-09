#include "minishell.h"

int	remove_env(int index)
{
	size_t	i;
	char	**tmp;

	i = 0;
	while (g_envs[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * i);
	if (!tmp)
		return (FAIL);
	i = 0;
	while (g_envs[i])
	{
		if (i != (size_t)index)
			tmp[i] = g_envs[i];
		i++;
	}
	tmp[i] = NULL;
	g_envs = tmp;
	return (SUCCESS);
}

int	ft_unset(char **argv)
{
	int	index;
	size_t	i;

	if (argv[1] != NULL)
	{
		i = 1;
		while (argv[i])
		{
			index = get_env_pos(argv[i]);
			if (index != -1)
			{
				if (remove_env(index) == FAIL)
					return (FAIL);
			}
			i++;
		}
	}
	return (SUCCESS);
}

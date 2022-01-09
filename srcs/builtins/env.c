#include "minishell.h"

int	ft_env(char **argv, t_env *envs)
{
	size_t	i;

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

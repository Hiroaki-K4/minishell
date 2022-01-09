#include "minishell.h"

int	ft_env(char **argv, t_global_state *state)
{
	size_t	i;

	if (argv[1] == NULL)
	{
		i = 0;
		while (state->envs->content[i])
		{
			if (ft_strchr(state->envs->content[i], '=') != NULL)
				printf("%s\n", state->envs->content[i]);
			i++;
		}
	}
	return (SUCCESS);
}

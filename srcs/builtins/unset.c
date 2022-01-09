#include "minishell.h"

int	remove_env(int index, t_global_state *state)
{
	size_t	i;
	char	**tmp;

	tmp = (char **)malloc(sizeof(char *) * state->envs->envs_num);
	if (!tmp)
		return (FAIL);
	i = 0;
	while (state->envs->content[i])
	{
		if (i != (size_t)index)
			tmp[i] = state->envs->content[i];
		i++;
	}
	tmp[i] = NULL;
	state->envs->content = tmp;
	return (SUCCESS);
}

int	ft_unset(char **argv, t_global_state *state)
{
	int	index;
	size_t	i;

	if (argv[1] != NULL)
	{
		i = 1;
		while (argv[i])
		{
			index = get_env_pos(argv[i], state);
			if (index != -1)
			{
				if (remove_env(index, state) == FAIL)
					return (FAIL);
			}
			i++;
		}
	}
	return (SUCCESS);
}

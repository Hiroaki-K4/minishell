#include "minishell.h"

char	**sort_envs(char **dup_env)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (dup_env[i])
	{
		j = i + 1;
		while (dup_env[j])
		{
			if (ft_strncmp(dup_env[i], dup_env[j], ft_strlen(dup_env[i]) + 1) > 0)
			{
				tmp = dup_env[i];
				dup_env[i] = dup_env[j];
				dup_env[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (dup_env);
}

int	init_envs(t_global_state *state, char **envp)
{
	size_t	i;

	state->envs = (t_env *)malloc(sizeof(t_env));
	i = 0;
	while (envp[i])
		i++;
	state->envs->content = (char **)malloc(sizeof(char *) * (i + 1));
	if (!state->envs->content)
		return (FAIL);
	i = 0;
	while (envp[i])
	{
		state->envs->content[i] = ft_strdup(envp[i]);
		i++;
	}
	state->envs->content[i] = NULL;
	state->envs->envs_num = i;
	return (SUCCESS);
}

char	*get_env(char *env, t_global_state *state)
{
	size_t	i;
	int	pos;
	char	*env_name;

	i = 0;
	while (state->envs->content[i])
	{
		pos = get_first_char_pos(state->envs->content[i], '=');
		if (pos != -1)
		{
			env_name = ft_substr(state->envs->content[i], 0, pos);
			if (ft_strncmp(env_name, env, ft_strlen(env_name)) == 0)
				return (ft_substr(state->envs->content[i], pos + 1, ft_strlen(state->envs->content[i]) - pos - 1));
		}
		i++;
	}
	return (NULL);
}
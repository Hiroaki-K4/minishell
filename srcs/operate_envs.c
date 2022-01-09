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

int	init_envs(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	g_envs = (char **)malloc(sizeof(char *) * (i + 1));
	if (!g_envs)
		return (FAIL);
	i = 0;
	while (envp[i])
	{
		g_envs[i] = ft_strdup(envp[i]);
		i++;
	}
	g_envs[i] = NULL;
	return (SUCCESS);
}

char	*get_env(char *env)
{
	size_t	i;
	int	pos;
	char	*env_name;

	i = 0;
	while (g_envs[i])
	{
		pos = get_first_char_pos(g_envs[i], '=');
		if (pos != -1)
		{
			env_name = ft_substr(g_envs[i], 0, pos);
			if (ft_strncmp(env_name, env, ft_strlen(env_name)) == 0)
				return (ft_substr(g_envs[i], pos + 1, ft_strlen(g_envs[i]) - pos - 1));
		}
		i++;
	}
	return (NULL);
}
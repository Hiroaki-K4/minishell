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

int	init_envs(t_envs **envs, char **envp)
{
	size_t	i;

	*envs = (t_envs *)malloc(sizeof(t_envs));
	i = 0;
	while (envp[i])
		i++;
	(*envs)->content = (char **)malloc(sizeof(char *) * (i + 1));
	if (!(*envs)->content)
		return (FAIL);
	i = 0;
	while (envp[i])
	{
		(*envs)->content[i] = ft_strdup(envp[i]);
		i++;
	}
	(*envs)->content[i] = NULL;
	(*envs)->envs_num = i;
	return (SUCCESS);
}

char	*get_env(char *env, t_envs *envs)
{
	size_t	i;
	int	pos;
	char	*env_name;

	i = 0;
	while (envs->content[i])
	{
		pos = get_first_char_pos(envs->content[i], '=');
		if (pos != -1)
		{
			env_name = ft_substr(envs->content[i], 0, pos);
			if (ft_strncmp(env_name, env, ft_strlen(env_name)) == 0)
				return (ft_substr(envs->content[i], pos + 1, ft_strlen(envs->content[i]) - pos - 1));
		}
		i++;
	}
	return (NULL);
}
#include "minishell.h"

char	**sort_envs(char **envs)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	i = 0;
	while (envs[i])
	{
		j = i + 1;
		while (envs[j])
		{
			if (ft_strncmp(envs[i], envs[j], ft_strlen(envs[i]) + 1) > 0)
			{
				tmp = envs[i];
				envs[i] = envs[j];
				envs[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (envs);
}

int	init_envs(t_envs **envs, char **envp)
{
	size_t	i;

	*envs = (t_envs *)malloc(sizeof(t_envs));
	if (!*envs)
		return (FAIL);
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
		if (!(*envs)->content[i])
			return (FAIL);
		i++;
	}
	(*envs)->content[i] = NULL;
	(*envs)->envs_num = i;
	return (SUCCESS);
}

int	get_env_name_and_value(char **name, char **value, char *env)
{
	int	pos;

	*name = NULL;
	*value = NULL;
	pos = get_first_char_pos(env, '=');
	if (pos != -1)
	{
		*name = ft_substr(env, 0, pos);
		*value = ft_substr(env, pos + 1, ft_strlen(env) - pos - 1);
		if (!*name || !*value)
			return (FAIL);
	}
	return (SUCCESS);
}

char	*get_env(char *name, t_envs *envs)
{
	size_t	i;
	char	*env_name;
	char	*env_value;

	i = 0;
	while (envs->content[i])
	{
		if (get_env_name_and_value(&env_name, &env_value, envs->content[i])
			== FAIL)
			return (NULL);
		if (!ft_strncmp(env_name, name, ft_strlen(env_name) + 1))
		{
			free(env_name);
			return (env_value);
		}
		free(env_name);
		free(env_value);
		i++;
	}
	return (NULL);
}

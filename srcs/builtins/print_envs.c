#include "minishell.h"

int	get_first_char_pos(char *word, char c)
{
	int pos;

	pos = 0;
	while (word[pos])
	{
		if (word[pos] == c)
			return (pos);
		pos++;
	}
	return (-1);
}

int	print_envs(t_envs *envs)
{
	size_t	i;
	char	**dup_env;
	char	*env_name;
	char	*env_value;

	dup_env = (char **)malloc(sizeof(char *) * (envs->envs_num + 1));
	if (!dup_env)
		return (FAIL);
	i = 0;
	while (envs->content[i])
	{
		if (ft_strchr(envs->content[i], '=') == NULL)
			dup_env[i] = envs->content[i];
		else
		{
			get_env_name_and_value(&env_name, &env_value, envs->content[i]);
			dup_env[i] = ft_strjoin(env_name, ft_strjoin(ft_strjoin("=\"", env_value), "\""));
		}
		if (!dup_env[i])
			return (FAIL);
		i++;
	}
	dup_env[i] = NULL;
	dup_env = sort_envs(dup_env);
	i = 0;
	while (dup_env[i])
	{
		printf("declare -x %s\n", dup_env[i]);
		i++;
	}
	return (SUCCESS);
}

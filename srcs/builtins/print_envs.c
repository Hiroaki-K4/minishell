#include "minishell.h"

// TODO: Add error handling

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
	char	*env_name;
	char	*env_value;
	char	**envs_for_print;

	envs_for_print = (char **)malloc(sizeof(char *) * (envs->envs_num + 1));
	if (!envs_for_print)
		return (FAIL);
	i = 0;
	while (envs->content[i])
	{
		if (ft_strchr(envs->content[i], '=') == NULL)
			envs_for_print[i] = envs->content[i];
		else
		{
			get_env_name_and_value(&env_name, &env_value, envs->content[i]);
			envs_for_print[i] = ft_strjoin(env_name, ft_strjoin(ft_strjoin("=\"", env_value), "\""));
		}
		if (!envs_for_print[i])
			return (FAIL);
		i++;
	}
	envs_for_print[i] = NULL;
	envs_for_print = sort_envs(envs_for_print);
	i = 0;
	while (envs_for_print[i])
	{
		printf("declare -x %s\n", envs_for_print[i]);
		i++;
	}
	return (SUCCESS);
}

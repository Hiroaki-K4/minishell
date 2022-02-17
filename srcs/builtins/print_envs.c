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
	char	*tmp;
	char	*tmp2;
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
			tmp = ft_strjoin("=\"", env_value);
			tmp2 = ft_strjoin(tmp, "\"");
			envs_for_print[i] = ft_strjoin(env_name, tmp2);
			free(tmp);
			free(tmp2);
			free(env_name);
			free(env_value);
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
	free_strings(envs_for_print);
	return (SUCCESS);
}

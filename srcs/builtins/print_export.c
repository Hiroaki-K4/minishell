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
			if (ft_strcmp(dup_env[i], dup_env[j]) > 0)
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

char	*make_env(char *env, int pos)
{
	char	*until_equal;
	char	*quoted_word;

	until_equal = ft_substr(env, 0, pos + 1);
	quoted_word = ft_strjoin(ft_strjoin("\"", ft_substr(env, pos + 1, ft_strlen(env) - pos)), "\"");
	return (ft_strjoin(until_equal, quoted_word));
}

int	print_export()
{
	size_t	i;
	int	pos;
	char	**dup_env;

	i = 0;
	while (g_envs[i])
		i++;
	dup_env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!dup_env)
		return (FAIL);
	i = 0;
	while (g_envs[i])
	{
		if (ft_strchr(g_envs[i], '=') == NULL)
			dup_env[i] = ft_strdup(g_envs[i]);
		else
		{
			pos = get_first_char_pos(g_envs[i], '=');
			dup_env[i] = make_env(g_envs[i], pos);
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

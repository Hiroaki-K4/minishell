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

char	*make_env(char *env, int pos)
{
	char	*until_equal;
	char	*quoted_word;

	until_equal = ft_substr(env, 0, pos + 1);
	quoted_word = ft_strjoin(ft_strjoin("\"", ft_substr(env, pos + 1, ft_strlen(env) - pos)), "\"");
	return (ft_strjoin(until_equal, quoted_word));
}

int	print_envs(t_global_state *state)
{
	size_t	i;
	int	pos;
	char	**dup_env;

	dup_env = (char **)malloc(sizeof(char *) * (state->envs->envs_num + 1));
	if (!dup_env)
		return (FAIL);
	i = 0;
	while (state->envs->content[i])
	{
		if (ft_strchr(state->envs->content[i], '=') == NULL)
			dup_env[i] = state->envs->content[i];
		else
		{
			pos = get_first_char_pos(state->envs->content[i], '=');
			dup_env[i] = make_env(state->envs->content[i], pos);
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

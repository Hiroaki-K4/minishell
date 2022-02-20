#include "minishell.h"

void	free_double_word(char *word1, char *word2)
{
	free(word1);
	free(word2);
}

int	handle_word_after_equal(char **argv, size_t i, t_envs **envs)
{
	size_t	j;
	char	*name;
	char	*val;

	j = 0;
	while (argv[i][j])
	{
		if (argv[i][j] == '=')
		{
			if (j == 0)
				return (FAIL);
			name = ft_substr(argv[i], 0, j);
			val = ft_substr(argv[i], j + 1, ft_strlen(argv[i]) - j);
			if (val == NULL)
				val = ft_strdup("");
			if (set_env(name, val, envs) == FAIL)
			{
				free_double_word(name, val);
				return (FAIL);
			}
			free_double_word(name, val);
		}
		j++;
	}
	return (SUCCESS);
}

int	set_new_env_to_envs(char **argv, t_envs **envs)
{
	size_t	i;
	char	*name;
	char	*val;

	i = 1;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '=') == NULL)
		{
			name = argv[i];
			val = NULL;
			if (set_env(name, val, envs) == FAIL)
				return (FAIL);
		}
		else
		{
			if (handle_word_after_equal(argv, i, envs) == FAIL)
				return (FAIL);
		}
		i++;
	}
	return (SUCCESS);
}

int	ft_export(char **argv, t_envs **envs)
{
	if (argv[1] == NULL)
	{
		if (print_envs(*envs) == FAIL)
			return (EXIT_FAILURE);
	}
	else
	{
		if (set_new_env_to_envs(argv, envs) == FAIL)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

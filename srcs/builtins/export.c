#include "minishell.h"

int	get_env_pos(char *env_name, t_global_state *state)
{
	size_t	i;
	size_t	j;
	char	*name;

	i = 0;
	while (state->envs->content[i])
	{
		j = 0;
		while (state->envs->content[i][j] && state->envs->content[i][j] != '=')
			j++;
		name = ft_substr(state->envs->content[i], 0, j);
		if (ft_strncmp(env_name, name, ft_strlen(env_name) + 1) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	set_env(char *name, char *val, t_global_state *state)
{
	int	env_pos;
	size_t	i;
	char	**new_environ;

	env_pos = get_env_pos(name, state);
	if (env_pos != -1)
	{
		if (val != NULL)
		{
			free(state->envs->content[env_pos]);
			state->envs->content[env_pos] = ft_strdup(ft_strjoin(ft_strjoin(name, "="), val));
		}
	}
	else
	{
		new_environ = (char **)malloc(sizeof(char *) * (state->envs->envs_num + 2));
		if (!new_environ)
			return (FAIL);
		i = 0;
		while (state->envs->content[i])
		{
			new_environ[i] = ft_strdup(state->envs->content[i]);
			free(state->envs->content[i]);
			i++;
		}
		free(state->envs->content);
		if (val == NULL)
			new_environ[i] = ft_strdup(name);
		else
			new_environ[i] = ft_strdup(ft_strjoin(ft_strjoin(name, "="), val));
		new_environ[i + 1] = NULL;
		state->envs->content = new_environ;
		state->envs->envs_num++;
	}
	return (SUCCESS);
}

int	handle_args(char **argv, t_global_state *state)
{
	size_t	i;
	size_t	j;
	char	*name;
	char	*val;

	i = 1;
	while (argv[i])
	{
		j = 0;
		if (ft_strchr(argv[i], '=') == NULL)
		{
			name = argv[i];
			val = NULL;
			if (set_env(name, val, state) == FAIL)
				return (FAIL);
		}
		else
		{
			while (argv[i][j])
			{
				if (argv[i][j] == '=')
				{
					if (j == 0)
						return (FAIL);
					else
					{
						name = ft_substr(argv[i], 0, j);
						val = ft_substr(argv[i], j + 1, ft_strlen(argv[i]) - j);
						if (val == NULL)
							val = ft_strdup("");
						if (val[0] == '"' && val[ft_strlen(val) - 1] == '"')
							val = ft_strtrim(val, "\"");
						else if (val[0] == '\'' && val[ft_strlen(val) - 1] == '\'')
							val = ft_strtrim(val, "'");
						if (set_env(name, val, state) == FAIL)
							return (FAIL);
					}
				}
				j++;
			}
		}
		i++;
	}
	return (SUCCESS);
}

int	ft_export(char **argv, t_global_state *state)
{
	if (argv[1] == NULL)
	{
		if (print_envs(state) == FAIL)
			return (FAIL);
	}
	else
	{
		if (handle_args(argv, state) == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}

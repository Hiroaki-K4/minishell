#include "minishell.h"

int	get_env_pos(char *env_name)
{
	size_t	i;
	size_t	j;
	char	*name;

	i = 0;
	while (g_envs[i])
	{
		j = 0;
		while (g_envs[i][j] && g_envs[i][j] != '=')
			j++;
		name = ft_substr(g_envs[i], 0, j);
		if (ft_strncmp(env_name, name, ft_strlen(env_name) + 1) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	ft_setenv(char *name, char *val)
{
	int	env_pos;
	size_t	i;
	size_t	size;
	char	**new_environ;

	env_pos = get_env_pos(name);
	if (env_pos != -1)
	{
		if (val != NULL)
		{
			free(g_envs[env_pos]);
			g_envs[env_pos] = ft_strdup(ft_strjoin(ft_strjoin(name, "="), val));
		}
	}
	else
	{
		size = 0;
		while (g_envs[size])
			size++;
		new_environ = (char **)malloc(sizeof(char *) * (size + 2));
		if (!new_environ)
			return (FAIL);
		i = 0;
		while (g_envs[i])
		{
			new_environ[i] = ft_strdup(g_envs[i]);
			free(g_envs[i]);
			i++;
		}
		free(g_envs);
		if (val == NULL)
			new_environ[i] = ft_strdup(name);
		else
			new_environ[i] = ft_strdup(ft_strjoin(ft_strjoin(name, "="), val));
		new_environ[i + 1] = NULL;
		g_envs = new_environ;
	}
	return (SUCCESS);
}

int	handle_args(char **argv)
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
			if (ft_setenv(name, val) == FAIL)
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
						if (ft_setenv(name, val) == FAIL)
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

int	ft_export(char **argv)
{
	if (argv[1] == NULL)
	{
		if (print_envs() == FAIL)
			return (FAIL);
	}
	else
	{
		if (handle_args(argv) == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}

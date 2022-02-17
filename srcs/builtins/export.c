#include "minishell.h"

int	get_env_pos(char *env_name, t_envs *envs)
{
	int		ret;
	size_t	i;
	size_t	j;
	char	*name;

	i = 0;
	ret = -1;
	while (envs->content[i])
	{
		j = 0;
		while (envs->content[i][j] && envs->content[i][j] != '=')
			j++;
		name = ft_substr(envs->content[i], 0, j);
		if (ft_strncmp(env_name, name, ft_strlen(env_name) + 1) == 0)
		{
			free(name);
			ret = i;
			break ;
		}
		free(name);
		i++;
	}
	return (ret);
}

int	set_env(char *name, char *val, t_envs **envs)
{
	int		env_pos;
	size_t	i;
	char	*tmp;
	char	**new_envs;

	env_pos = get_env_pos(name, *envs);
	if (env_pos != -1)
	{
		if (val != NULL)
		{
			free((*envs)->content[env_pos]);
			tmp = ft_strjoin(name, "=");
			(*envs)->content[env_pos] = ft_strjoin(tmp, val);
			free(tmp);
		}
	}
	else
	{
		new_envs = (char **)malloc(sizeof(char *) * ((*envs)->envs_num + 2));
		if (!new_envs)
			return (FAIL);
		i = 0;
		while ((*envs)->content[i])
		{
			new_envs[i] = (*envs)->content[i];
			i++;
		}
		if (val == NULL)
			new_envs[i] = ft_strdup(name);
		else
		{
			tmp = ft_strjoin(name, "=");
			new_envs[i] = ft_strjoin(tmp, val);
			free(tmp);
		}
		new_envs[i + 1] = NULL;
		free((*envs)->content);
		(*envs)->content = new_envs;
		(*envs)->envs_num++;
	}
	return (SUCCESS);
}

int	set_new_env_to_envs(char **argv, t_envs **envs)
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
			if (set_env(name, val, envs) == FAIL)
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
						if (set_env(name, val, envs) == FAIL)
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

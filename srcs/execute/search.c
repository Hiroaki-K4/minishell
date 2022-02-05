#include "minishell.h"

static char	**get_candidate_paths(char *path, t_envs *envs)
{
	char	**paths;
	char	*tmp;

	if (path == NULL)
		return (NULL);
	tmp = get_env("PATH", envs);
	if (tmp == NULL)
		return (NULL);
	paths = ft_split(tmp, ':');
	free(tmp);
	if (paths == NULL)
		return (NULL);
	return (paths);
}

static char	*get_executable_path(char *path, char **paths)
{
	char	*tmp;
	char	*tmp2;
	char	*executable_path;

	executable_path = NULL;
	while (*paths)
	{
		tmp2 = ft_strjoin(*paths, "/");
		tmp = ft_strjoin(tmp2, path);
		free(tmp2);
		if (access(tmp, X_OK) == SUCCESS)
		{
			free(executable_path);
			executable_path = tmp;
		}
		else
			free(tmp);
		paths++;
	}
	if (executable_path == NULL)
		executable_path = path;
	return (executable_path);
}

char	*search(char *path, t_envs *envs)
{
	int		i;
	char	**paths;
	char	*executable_path;

	i = 0;
	executable_path = NULL;
	paths = get_candidate_paths(path, envs);
	if (paths == NULL)
		return (NULL);
	executable_path = get_executable_path(path, paths);
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	return (executable_path);
}

#include "minishell.h"

char	*search(char *path, t_envs *envs)
{
	char	*tmp;
	char	**paths;
	char	*executable_path;

	if (path == NULL)
		return (NULL);
	executable_path = NULL;
	tmp = get_env("PATH", envs);
	if (tmp == NULL)
		return (NULL);
	paths = ft_split(tmp, ':');
	free(tmp);
	if (paths == NULL)
		return (NULL);
	while (*paths)
	{
		tmp = ft_strjoin(*paths, ft_strjoin("/", path)); // TODO: i think this will leak
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

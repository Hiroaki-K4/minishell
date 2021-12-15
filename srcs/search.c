#include "minishell.h"

char	**get_paths(char **envp)
{
	char	**paths;

	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH=", 5))
			break ;
		envp++;
	}
	paths = ft_split(*envp, ':');
	return (paths);
}

char	*search(char *path, char **envp)
{
	char	*tmp;
	char	**paths;
	char	*executable_path;

	executable_path = NULL;
	paths = get_paths(envp);
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

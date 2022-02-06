#include "minishell.h"

void	free_strings(char **strings)
{
	size_t	i;

	i = 0;
	if (strings == NULL)
		return ;
	while (strings[i])
	{
		free(strings[i]);
		strings[i] = NULL;
		i++;
	}
	free(strings);
}

static char	*make_tmp_path(char **argv, char *path)
{
	char	*tmp;
	char	*tmp2;

	if (path[ft_strlen(path) - 1] == '/')
		tmp = ft_strjoin(path, argv[1]);
	else
	{
		tmp2 = ft_strjoin(path, "/");
		tmp = ft_strjoin(tmp2, argv[1]);
		free(tmp2);
	}
	return (tmp);
}

static void	concat_cdpath(char **argv, char **curpath, char *cdpath)
{
	size_t	i;
	char	*tmp;
	char	**paths;

	i = 0;
	paths = ft_split(cdpath, ':');
	if (paths == NULL)
		return ;
	while (paths[i])
	{
		tmp = make_tmp_path(argv, paths[i]);
		if (access(tmp, X_OK) == SUCCESS)
		{
			*curpath = tmp;
			break ;
		}
		free(tmp);
		i++;
	}
	free_strings(paths);
}

static int	is_relative_path(char **argv)
{
	return (!ft_strncmp(argv[1], ".", ft_strlen(argv[1]) + 1)
		|| !ft_strncmp(argv[1], "..", ft_strlen(argv[1]) + 1));
}

void	set_curpath(char **argv, t_envs **envs, char **curpath)
{
	char	*tmp;
	char	*cdpath;

	if (*argv[1] == '/')
		*curpath = ft_strdup(argv[1]);
	else
	{
		if (!is_relative_path(argv))
		{
			cdpath = get_env("CDPATH", *envs);
			if (cdpath == NULL || !*cdpath)
			{
				tmp = ft_strjoin("./", argv[1]);
				if (access(tmp, X_OK) == SUCCESS)
					*curpath = tmp;
				else
					free(tmp);
			}
			else
				concat_cdpath(argv, curpath, cdpath);
			free(cdpath);
		}
		if (*curpath == NULL)
			*curpath = ft_strdup(argv[1]);
	}
}

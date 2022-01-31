#include "minishell.h"

static void	concat_cdpath(char **argv, char **curpath, char *cdpath)
{
	size_t	i;
	char	*tmp;
	char	*tmp2;
	char	**paths;

	i = 0;
	paths = ft_split(cdpath, ':');
	if (paths == NULL || !*cdpath)
		return ;
	while (paths[i])
	{
		if (paths[i][ft_strlen(*paths) - 1] == '/')
			tmp = ft_strjoin(paths[i], argv[1]);
		else
		{
			tmp2 = ft_strjoin(paths[i], "/");
			tmp = ft_strjoin(tmp2, argv[1]);
			free(tmp2);
		}
		if (access(tmp, X_OK) == SUCCESS)
		{
			*curpath = tmp;
			break ;
		}
		free(tmp);
		free(paths[i]);
		i++;
	}
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

static void	set_curpath(char **argv, t_envs **envs, char **curpath)
{
	char	*tmp;
	char	*cdpath;

	if (*argv[1] == '/')
		*curpath = ft_strdup(argv[1]);
	else
	{
		if (*argv[1] != '.'
			&& ft_strncmp(argv[1], "..", ft_strlen(argv[1]) + 1))
		{
			cdpath = get_env("CDPATH", *envs);
			if (cdpath == NULL)
			{
				tmp = ft_strjoin("./", argv[1]);
				if (access(tmp, X_OK) == SUCCESS)
					*curpath = tmp;
			}
			else
				concat_cdpath(argv, curpath, cdpath);
		}
		if (*curpath == NULL)
			*curpath = ft_strdup(argv[1]);
	}
}

static void	concat_pwd(t_envs **envs, char **curpath)
{
	char	*pwd;
	char	*tmp;
	char	*tmp2;

	pwd = NULL;
	tmp = NULL;
	tmp2 = NULL;
	if (**curpath != '/')
	{
		pwd = get_env("PWD", *envs);
		if (pwd[ft_strlen(pwd) - 1] == '/')
		{
			tmp = ft_strjoin(pwd, *curpath);
		}
		else
		{
			tmp2 = ft_strjoin(pwd, "/");
			tmp = ft_strjoin(tmp2, *curpath);
			free(tmp2);
		}
		free(*curpath);
		*curpath = tmp;
	}
}

static void	convert_curpath_to_canonical_form(char **curpath)
{
	(void)curpath;
	return ;
}

int	ft_cd(char **argv, t_envs **envs, int *exit_status)
{
	char	*tmp;
	char	*curpath;

	(void)envs;
	*exit_status = 0;
	curpath = NULL;
	if (argv[1] == NULL)
	{
		tmp = get_env("HOME", *envs);
		if (tmp == NULL)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
		argv[1] = tmp;
	}
	set_curpath(argv, envs, &curpath);
	concat_pwd(envs, &curpath);
	convert_curpath_to_canonical_form(&curpath);
	if (chdir(curpath) == FAIL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		free(curpath);
		*exit_status = 1;
		return (1);
	}
	set_env("OLDPWD", get_env("PWD", *envs), envs);
	set_env("PWD", curpath, envs);
	free(curpath);
	return (0);
}

#include "minishell.h"

static void	print_error(char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

static int	set_home(char **argv, t_envs *envs)
{
	char	*tmp;

	tmp = get_env("HOME", envs);
	if (tmp == NULL)
		return (FAIL);
	argv[1] = tmp;
	return (SUCCESS);
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
			tmp = ft_strjoin(pwd, *curpath);
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
	char	*curpath;

	(void)envs;
	*exit_status = 0;
	curpath = NULL;
	if (argv[1] == NULL && set_home(argv, *envs) == FAIL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	set_curpath(argv, envs, &curpath);
	concat_pwd(envs, &curpath);
	convert_curpath_to_canonical_form(&curpath);
	if (chdir(curpath) == FAIL)
	{
		print_error(argv[1]);
		free(curpath);
		*exit_status = 1;
		return (1);
	}
	set_env("OLDPWD", get_env("PWD", *envs), envs);
	set_env("PWD", curpath, envs);
	free(curpath);
	return (0);
}

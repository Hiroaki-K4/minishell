/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:47:26 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/21 22:47:27 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_error(char *path)
{
	char	*tmp;
	char	*err_msg;

	tmp = ft_strdup("minishell: cd: ");
	err_msg = ft_strjoin(tmp, path);
	free(tmp);
	tmp = err_msg;
	err_msg = ft_strjoin(tmp, ": ");
	free(tmp);
	tmp = err_msg;
	err_msg = ft_strjoin(tmp, strerror(errno));
	free(tmp);
	tmp = err_msg;
	err_msg = ft_strjoin(tmp, "\n");
	ft_putstr_fd(err_msg, 2);
	free(tmp);
	free(err_msg);
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
		free(pwd);
		free(*curpath);
		*curpath = tmp;
	}
}

static void	convert_curpath_to_canonical_form(char **curpath)
{
	(void)curpath;
	return ;
}

int	ft_cd(char **argv, t_envs **envs)
{
	char	*tmp;
	char	*curpath;

	curpath = NULL;
	if (argv[1] == NULL && set_home(argv, *envs) == FAIL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (EXIT_FAILURE);
	}
	set_curpath(argv, envs, &curpath);
	concat_pwd(envs, &curpath);
	convert_curpath_to_canonical_form(&curpath);
	if (chdir(curpath) == FAIL)
	{
		print_error(argv[1]);
		free(curpath);
		return (EXIT_FAILURE);
	}
	tmp = get_env("PWD", *envs);
	set_env("OLDPWD", tmp, envs);
	set_env("PWD", curpath, envs);
	free(tmp);
	free(curpath);
	return (EXIT_SUCCESS);
}

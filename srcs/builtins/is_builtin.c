/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:47:34 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/21 22:47:35 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_builtin_command(char **argv, t_envs **envs, int *exit_status)
{
	int		ret;

	if (argv == NULL || argv[0] == NULL)
		return (FALSE);
	ret = 0;
	if (!ft_strncmp(argv[0], "cd", ft_strlen("cd") + 1))
		ret = ft_cd(argv, envs);
	else if (!ft_strncmp(argv[0], "export", ft_strlen("export") + 1))
		ret = ft_export(argv, envs);
	else if (!ft_strncmp(argv[0], "unset", ft_strlen("unset") + 1))
		ret = ft_unset(argv, envs);
	else if (!ft_strncmp(argv[0], "exit", ft_strlen("exit") + 1))
		ret = ft_exit(argv, envs);
	else
		return (FALSE);
	*exit_status = ret;
	return (TRUE);
}

int	is_builtin_command(char **argv, t_envs *envs, int *exit_status)
{
	int		ret;

	if (argv == NULL || argv[0] == NULL)
		return (FALSE);
	ret = 0;
	if (!ft_strncmp(argv[0], "echo", ft_strlen("echo") + 1))
		ret = ft_echo(argv, envs);
	else if (!ft_strncmp(argv[0], "pwd", ft_strlen("pwd") + 1))
		ret = ft_pwd(argv, envs);
	else if (!ft_strncmp(argv[0], "env", ft_strlen("env") + 1))
		ret = ft_env(argv, envs);
	else
		return (FALSE);
	*exit_status = ret;
	return (TRUE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychida <ychida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:47:34 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/27 21:09:28 by ychida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	prelude(t_global_state *state, int special_fds[3])
{
	size_t	i;

	i = 0;
	special_fds[0] = dup(0);
	special_fds[1] = dup(1);
	special_fds[2] = dup(2);
	while (i < state->redirect_num)
	{
		dup2(state->redirects[i]->file_fd, state->redirects[i]->redirect_fd);
		i++;
	}
}

static void	postlude(t_global_state *state, int special_fds[3])
{
	size_t	i;

	i = 0;
	while (i < state->redirect_num)
	{
		close(state->redirects[i]->redirect_fd);
		i++;
	}
	dup2(special_fds[0], 0);
	dup2(special_fds[1], 1);
	dup2(special_fds[2], 2);
}

int	is_special_builtin_command(char **argv, t_global_state *state)
{
	int		ret;
	t_envs	**envs;
	int		special_fds[3];

	envs = &state->envs;
	if (argv == NULL || argv[0] == NULL)
		return (FALSE);
	prelude(state, special_fds);
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
	{
		postlude(state, special_fds);
		return (FALSE);
	}
	state->last_command_exit_status = ret;
	postlude(state, special_fds);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:47:41 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/21 22:47:42 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define BUF_SIZE 1024

int	ft_pwd(char **argv, t_envs *envs)
{
	char	*buf;

	(void)argv;
	(void)envs;
	buf = getcwd(NULL, 0);
	if (buf == NULL)
		buf = get_env("PWD", envs);
	else
		set_env("PWD", buf, &envs);
	printf("%s\n", buf);
	free(buf);
	return (SUCCESS);
}

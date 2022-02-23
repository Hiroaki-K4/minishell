/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:47:02 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/23 14:41:22 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_all_digit(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '-' || str[i] == '+') && i == 0)
		{
			i++;
			continue ;
		}
		if (ft_isdigit(str[i]) == 0)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static	long	calc(const char *str, long cutoff, long cutlim, int is_minus)
{
	long	res;

	res = 0;
	while ('0' <= *str && *str <= '9')
	{
		if (is_minus)
		{
			if (res < cutoff || (res == cutoff && (*str - '0') > cutlim))
				return (TRUE);
			else
				res = 10 * res - (*str - '0');
		}
		else
		{
			if (res > cutoff || (res == cutoff && (*str - '0') > cutlim))
				return (TRUE);
			else
				res = 10 * res + (*str - '0');
		}
		str++;
	}
	return (FALSE);
}

static	long	check_over_long(const char *str, int is_minus)
{
	long	cutoff;
	long	cutlim;
	long	res;

	if (is_minus)
		cutoff = LONG_MIN;
	else
		cutoff = LONG_MAX;
	cutlim = cutoff % 10;
	cutoff /= 10;
	if (is_minus)
	{
		if (cutlim > 0)
		{
			cutlim -= 10;
			cutoff += 1;
		}
		cutlim = -cutlim;
	}
	res = calc(str, cutoff, cutlim, is_minus);
	return (res);
}

int	is_over_long(const char *str)
{
	long	res;
	long	is_minus;

	is_minus = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n' || \
			*str == '\f' || *str == '\r' || *str == '\v')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			is_minus = 1;
		str++;
	}
	res = check_over_long(str, is_minus);
	return (res);
}

int	ft_exit(char **argv, t_envs **envs)
{
	(void)envs;
	ft_putendl_fd("exit", 1);
	if (argv[1] == NULL)
		exit(EXIT_SUCCESS);
	else if (is_all_digit(argv[1]) == FALSE || is_over_long(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 1);
		ft_putstr_fd(argv[1], 1);
		ft_putendl_fd(": numeric argument required", 1);
		exit(255);
	}
	else if (argv[2] != NULL)
	{
		ft_putendl_fd("minishell: exit: too many arguments", 1);
		return (EXIT_FAILURE);
	}
	exit(ft_atoi(argv[1]));
}

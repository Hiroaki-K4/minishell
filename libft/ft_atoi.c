/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 17:07:21 by hkubo             #+#    #+#             */
/*   Updated: 2021/06/05 17:31:07 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int		i;
	int		flag;
	int		ans;

	flag = 1;
	i = 0;
	while (nptr[i] == ' ' || nptr[i] == '\f' || nptr[i] == '\n'
		|| nptr[i] == '\r' || nptr[i] == '\t' || nptr[i] == '\v')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			flag = -1;
		i++;
	}
	ans = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		ans = (ans * 10) + (nptr[i] - '0');
		i++;
	}
	ans = ans * flag;
	return (ans);
}

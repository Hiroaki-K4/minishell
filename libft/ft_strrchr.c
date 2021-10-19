/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 15:39:32 by hkubo             #+#    #+#             */
/*   Updated: 2021/06/05 17:24:51 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char const	*ptr_s;
	int					i;
	int					count;

	count = -1;
	ptr_s = (unsigned char const *)s;
	i = 0;
	while (ptr_s[i] != '\0')
	{
		if (ptr_s[i] == (unsigned char)c)
			count = i;
		i++;
	}
	if (ptr_s[i] == '\0' && ptr_s[i] == (unsigned char)c)
		return ((char *)(s + i));
	if (count != -1)
		return ((char *)(s + count));
	return (NULL);
}

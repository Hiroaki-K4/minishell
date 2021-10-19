/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 14:42:16 by hkubo             #+#    #+#             */
/*   Updated: 2021/06/05 17:15:56 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char const	*ptr_s;
	int					i;

	ptr_s = (unsigned char const *)s;
	i = 0;
	while (ptr_s[i] != '\0')
	{
		if (ptr_s[i] == (unsigned char)c)
			return ((char *)(s + i));
		i++;
	}
	if (ptr_s[i] == '\0' && ptr_s[i] == (unsigned char)c)
		return ((char *)(s + i));
	return (NULL);
}

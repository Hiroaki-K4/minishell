/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 22:42:31 by hkubo             #+#    #+#             */
/*   Updated: 2021/06/05 17:20:38 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	unsigned char		*ptr_dst;
	unsigned char const	*ptr_src;
	size_t				i;

	ptr_dst = (unsigned char *)dest;
	ptr_src = (unsigned char const *)src;
	i = 0;
	while (i < n)
	{
		ptr_dst[i] = ptr_src[i];
		if (ptr_dst[i] == (unsigned char)c)
			return ((void *)(dest + i + 1));
		i++;
	}
	return (NULL);
}

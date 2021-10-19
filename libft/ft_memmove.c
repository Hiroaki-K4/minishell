/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 11:07:30 by hkubo             #+#    #+#             */
/*   Updated: 2020/11/28 19:40:01 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*ptr_dst;
	unsigned char const	*ptr_src;

	ptr_dst = (unsigned char *)dest;
	ptr_src = (unsigned char *)src;
	if (ptr_src < ptr_dst)
	{
		while (n > 0)
		{
			ptr_dst[n - 1] = ptr_src[n - 1];
			n--;
		}
	}
	else
	{
		while (n > 0)
		{
			*(ptr_dst++) = *(ptr_src++);
			n--;
		}
	}
	return (dest);
}

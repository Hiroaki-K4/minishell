/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/11 22:45:38 by hkubo             #+#    #+#             */
/*   Updated: 2020/11/29 09:37:17 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*p_dest;
	unsigned char const	*p_src;

	p_dest = (unsigned char *)(dest);
	p_src = (unsigned char const *)(src);
	while (n > 0)
	{
		*p_dest++ = *p_src++;
		n--;
	}
	return (dest);
}

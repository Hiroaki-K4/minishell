/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 22:39:47 by hkubo             #+#    #+#             */
/*   Updated: 2020/11/29 09:35:34 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *buf, int ch, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)buf;
	while (n > 0)
	{
		*p = (unsigned char)ch;
		p++;
		n--;
	}
	return (buf);
}

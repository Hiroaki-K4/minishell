/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 20:35:50 by hkubo             #+#    #+#             */
/*   Updated: 2020/11/28 19:38:17 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d_size;
	size_t	i;

	d_size = 0;
	while (dst[d_size] != '\0' && d_size < size)
		d_size++;
	i = 0;
	while (src[i] != '\0' && (d_size + i + 1) < size)
	{
		dst[d_size + i] = src[i];
		i++;
	}
	if (d_size < size)
		dst[d_size + i] = '\0';
	return (d_size + ft_strlen(src));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 13:24:16 by hkubo             #+#    #+#             */
/*   Updated: 2021/06/05 17:07:54 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	n;

	n = ft_strlen(little);
	if (n == 0)
		return ((char *)(big));
	i = 0;
	while (big[i] != '\0' && i < len)
	{
		if (ft_strncmp(big + i, little, n) == 0 && i + n <= len)
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}

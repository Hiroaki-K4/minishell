/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 08:52:17 by hkubo             #+#    #+#             */
/*   Updated: 2021/12/30 22:49:08 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*new_ptr;
	unsigned int	i;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
	{
		new_ptr = (char *)malloc(sizeof(char) * 1);
		if (!new_ptr)
			return (NULL);
		new_ptr[0] = '\0';
		return (new_ptr);
	}
	new_ptr = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_ptr)
		return (NULL);
	i = 0;
	while (i < len && s[start + i] != '\0')
	{
		new_ptr[i] = s[start + i];
		i++;
	}
	new_ptr[i] = '\0';
	return (new_ptr);
}

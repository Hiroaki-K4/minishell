/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 12:19:02 by hkubo             #+#    #+#             */
/*   Updated: 2021/06/05 17:29:26 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	size_t			len;
	char			*ans_ptr;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	ans_ptr = (char *)malloc(sizeof(char) * (len + 1));
	if (!ans_ptr)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		ans_ptr[i] = (*f)(i, s[i]);
		i++;
	}
	ans_ptr[i] = '\0';
	return (ans_ptr);
}

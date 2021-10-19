/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 09:09:12 by hkubo             #+#    #+#             */
/*   Updated: 2021/06/05 17:17:04 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	new_str_len;
	char	*new_str;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	new_str_len = ft_strlen(s1) + ft_strlen(s2);
	new_str = (char *)malloc(sizeof(char) * (new_str_len + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[j] != '\0')
		new_str[i++] = s1[j++];
	j = 0;
	while (s2[j] != '\0')
		new_str[i++] = s2[j++];
	new_str[i] = '\0';
	return (new_str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 22:42:10 by hkubo             #+#    #+#             */
/*   Updated: 2021/06/05 17:24:06 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	num_str(char const *s1, char c)
{
	int	count;
	int	c_flag;

	if (*s1 == '\0')
		return (0);
	count = 0;
	c_flag = 0;
	while (*s1 != '\0')
	{
		if (*s1 == c)
			c_flag = 0;
		else if (c_flag == 0)
		{
			count++;
			c_flag = 1;
		}
		s1++;
	}
	return (count);
}

static	char	*core_div(char const *s, char *div, int start, int goal)
{
	char	*tmp_str;
	char	*div_ptr;

	tmp_str = ft_substr(s, start, goal);
	div_ptr = ft_strtrim(tmp_str, div);
	free(tmp_str);
	return (div_ptr);
}

static	char	**main_div(char const *s, char c, char *div, char **ptr)
{
	int		j;
	int		k;
	int		flag;
	int		start;

	j = -1;
	k = 0;
	flag = 0;
	start = 0;
	while (s[++j] != '\0')
	{
		if (s[j] == c && flag == 1)
		{
			ptr[k++] = core_div(s, div, start, j - start);
			flag = 0;
			start = j + 1;
		}
		if (flag == 0 && s[j] != c)
			flag = 1;
		if (s[j] != c && s[j + 1] == '\0')
			ptr[k++] = core_div(s, div, start, j - start + 1);
	}
	ptr[k] = NULL;
	return (ptr);
}

char	**ft_split(char const *s, char c)
{
	char	div[2];
	char	**ptr_ptr;

	if (s == NULL)
		return (NULL);
	ptr_ptr = (char **)malloc(sizeof(char *) * (num_str(s, c) + 1));
	if (!ptr_ptr)
		return (NULL);
	div[0] = c;
	div[1] = '\0';
	return (main_div(s, c, div, ptr_ptr));
}

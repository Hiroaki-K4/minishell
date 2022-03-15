/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychida <ychida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 23:37:36 by ychida            #+#    #+#             */
/*   Updated: 2022/03/15 23:55:05 by ychida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	convert(char **splitted, char ***to_be_joined)
{
	size_t	i;
	size_t	cursor;

	i = 0;
	cursor = 0;
	while (splitted[i])
	{
		if (ft_strncmp(splitted[i], ".", 2) != 0
			&& ft_strncmp(splitted[i], "..", 3) != 0 && splitted[i][0] != '\0')
		{
			(*to_be_joined)[cursor] = ft_strdup(splitted[i]);
			cursor++;
		}
		else if (ft_strncmp(splitted[i], "..", 3) == 0)
		{
			if (cursor > 0)
			{
				cursor--;
				free((*to_be_joined)[cursor]);
				(*to_be_joined)[cursor] = NULL;
			}
		}
		i++;
	}
	(*to_be_joined)[cursor] = NULL;
}

void	convert_curpath_to_canonical_form(char **curpath)
{
	size_t	i;
	char	*tmp;
	char	**splitted;
	char	**to_be_joined;

	i = 0;
	splitted = ft_split(*curpath, '/');
	to_be_joined = (char **)malloc(sizeof(char *) * ft_strlen(*curpath));
	free(*curpath);
	*curpath = ft_strdup("/");
	convert(splitted, &to_be_joined);
	while (to_be_joined[i])
	{
		tmp = ft_strjoin(*curpath, to_be_joined[i]);
		free(*curpath);
		*curpath = tmp;
		if (to_be_joined[i + 1] == NULL)
			break ;
		tmp = ft_strjoin(*curpath, "/");
		free(*curpath);
		*curpath = tmp;
		i++;
	}
	free_strings(splitted);
	free_strings(to_be_joined);
}

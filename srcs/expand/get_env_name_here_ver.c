/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_name_here_ver.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 21:20:23 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/27 21:20:43 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_name_here_ver(char *input, size_t *current_pos)
{
	size_t	start;

	start = *current_pos;
	while (input[*current_pos]
		&& input[*current_pos] != ' '
		&& input[*current_pos] != '$'
		&& input[*current_pos] != '\''
		&& input[*current_pos] != '\"'
		&& input[*current_pos] != ':'
	)
	{
		(*current_pos)++;
		if (input[*current_pos - 1] == '?')
			return (ft_strdup("$?"));
	}
	if (*current_pos == start)
		return (ft_strdup("$"));
	return (ft_substr(input, start, *current_pos - start));
}

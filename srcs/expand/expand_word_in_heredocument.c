/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_in_heredocument.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 17:00:30 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/27 19:22:38 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_name_here_ver(char *input, size_t *current_pos)
{
    size_t  start;

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

char    *check_name_here_ver(char *name, t_envs *envs, int exit_status)
{
	char	*value;

	if (!ft_strncmp(name, "$", ft_strlen(name) + 1))
		return (ft_strdup(name));
	else if (!ft_strncmp(name, "$?", ft_strlen(name) + 1))
		return (ft_itoa(exit_status));
	else
	{
		value = get_env(name, envs);
        return (value);
    }
}

char    *expand_env_vals_core_here_ver(char *input, size_t *current_pos, t_envs *envs, int exit_status)
{
    char    *name;
    char    *expanded;

    (*current_pos)++;
    name = get_env_name_here_ver(input, current_pos);
	if (!name)
		return (NULL);
	expanded = check_name_here_ver(name, envs, exit_status);
	free(name);
    return (expanded);
}

char    *expand_env_vals_here_ver(char *input, t_envs *envs, int exit_status)
{
    size_t  current_pos;
    size_t  start;
    char    *expanded;
    char    *tmp;
    char    *result;
    char    *add_word;

    result = ft_strdup("");
    current_pos = 0;
    start = 0;
	while (input[current_pos])
	{
		if (input[current_pos] == '$')
		{
            if (start != current_pos)
            {
                add_word = ft_substr(input, start, current_pos - start);
                tmp = ft_strjoin(result, add_word);
                free(result);
                free(add_word);
                result = tmp;
            }
			expanded = expand_env_vals_core_here_ver(input, &current_pos, envs, exit_status);
            start = current_pos;
            tmp = ft_strjoin(result, expanded);
            free(result);
            free(expanded);
            result = tmp;
            continue ;
		}
		current_pos++;
	}
    if (start != current_pos)
    {
        add_word = ft_substr(input, start, current_pos - start);
        tmp = ft_strjoin(result, add_word);
        free(result);
        free(add_word);
        result = tmp;
    }
	return (result);
}

char    *expand_word_in_heredocument(char *input, t_envs *envs, int exit_status)
{
    char    *expanded;
    if (ft_strchr(input, '$') != NULL)
    {
        expanded = expand_env_vals_here_ver(input, envs, exit_status);
        return (expanded);
    }
    return (ft_strdup(input));
}

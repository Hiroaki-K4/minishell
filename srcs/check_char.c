/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:26:33 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/20 21:26:35 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_metacharacter_with_token_kind(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_metacharacter_without_token_kind(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_separating_word(char *line, int pos)
{
	if (is_metacharacter_with_token_kind(line[pos]))
		return (1);
	else if ((ft_isdigit(line[pos]) && (line[pos + 1] == '<'
				|| line[pos + 1] == '>')))
		return (1);
	return (0);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_redirect_char(char c)
{
	return (c == '<' || c == '>');
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:25:35 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/27 20:21:51 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quote(t_expand_state *e_state, t_token *token,
	char *progress, int *q_removed)
{
	char	*quote_removed;
	char	*extracted;

	extracted = ft_substr(token->content, e_state->start + 1,
			e_state->current_pos - e_state->start - 1);
	if (!extracted)
		return (NULL);
	quote_removed = ft_strjoin(progress, extracted);
	if (!quote_removed)
		return (NULL);
	free(extracted);
	e_state->quote_state = NORMAL;
	*q_removed = TRUE;
	return (quote_removed);
}

char	*get_word_in_quote(t_token *token, t_expand_state *e_state,
	char *word, int *q_removed)
{
	char	*tmp;

	if (token->content[e_state->current_pos] == '\''
		&& e_state->quote_state == IN_QUOTE)
		tmp = remove_quote(e_state, token, word, q_removed);
	else if (token->content[e_state->current_pos] == '\"'
		&& e_state->quote_state == IN_DQUOTE)
		tmp = remove_quote(e_state, token, word, q_removed);
	else
		tmp = ft_strdup(word);
	if (!tmp)
		return (NULL);
	return (tmp);
}

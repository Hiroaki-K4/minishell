/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:25:00 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/20 21:25:01 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_q_state(t_expand_state *e_state, char quote)
{
	if (quote == '\'')
		e_state->quote_state = IN_QUOTE;
	else if (quote == '\"')
		e_state->quote_state = IN_DQUOTE;
	e_state->start = e_state->current_pos;
	e_state->current_pos++;
}

char	*read_current_char(char *content, size_t current_pos, char *q_removed)
{
	char	*current_char;
	char	*str;

	current_char = ft_substr(content, current_pos, 1);
	if (!current_char)
		return (NULL);
	str = ft_strjoin(q_removed, current_char);
	if (!str)
		return (NULL);
	free(current_char);
	return (str);
}

char	*get_q_removed(t_expand_state *e_state, t_token *token,
		char *word, char *tmp)
{
	while (token->content[e_state->current_pos])
	{
		if (e_state->quote_state == NORMAL)
		{
			if (is_quote(token->content[e_state->current_pos]))
			{
				change_q_state(e_state, token->content[e_state->current_pos]);
				continue ;
			}
			tmp = read_current_char(token->content, e_state->current_pos, word);
		}
		else
		{
			if (!is_quote(token->content[e_state->current_pos]))
			{
				e_state->current_pos++;
				continue ;
			}
			tmp = get_word_in_quote(token, e_state, word);
		}
		free(word);
		word = tmp;
		e_state->current_pos++;
	}
	return (word);
}

t_token	*make_quote_removed_token(t_expand_state *e_state, t_token *token)
{
	t_token			*q_removed;
	char			*quote_removed;
	char			*empty;

	empty = ft_strdup("");
	quote_removed = get_q_removed(e_state, token, empty, NULL);
	if (!quote_removed)
		return (NULL);
	q_removed = make_token(quote_removed, 0, ft_strlen(quote_removed),
			token->attr);
	if (!q_removed)
		return (NULL);
	free(quote_removed);
	return (q_removed);
}

t_list	*check_quote(t_expand_state *e_state)
{
	t_token			*q_removed;
	t_token			*token;
	t_list			*expanded_list;
	t_list			*tmp_list;

	expanded_list = NULL;
	while (e_state->token_list != NULL)
	{
		token = (t_token *)e_state->token_list->content;
		init_expand_state(e_state);
		q_removed = make_quote_removed_token(e_state, token);
		if (!q_removed)
			return (NULL);
		if (ft_lstadd_token(&expanded_list, q_removed) == FAIL)
			return (NULL);
		tmp_list = e_state->token_list->next;
		ft_lstdelone_all(e_state->token_list, free);
		e_state->token_list = tmp_list;
	}
	return (expanded_list);
}

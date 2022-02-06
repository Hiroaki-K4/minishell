#include "minishell.h"

char	*update_state_with_quote(t_expand_state *e_state, t_token *token,
		t_quote_state quote_state, char *progress)
{
	char	*quote_removed;
	char	*extracted;

	if (e_state->quote_state == NORMAL)
	{
		e_state->quote_state = quote_state;
		e_state->start = e_state->current_pos;
	}
	else if (e_state->quote_state == quote_state)
	{
		extracted = ft_substr(token->content, e_state->start + 1,
				e_state->current_pos - e_state->start - 1);
		quote_removed = ft_strjoin(progress, extracted);
		free(extracted);
		e_state->quote_state = NORMAL;
		return (quote_removed);
	}
	return (ft_strdup(progress));
}

t_token	*remove_quote_core(t_expand_state *e_state)
{
	char	*quote_removed;
	char	*tmp;
	char	*extracted;
	t_token	*q_removed;
	t_token	*token;

	q_removed = NULL;
	quote_removed = ft_strdup("");
	init_expand_state(e_state);
	token = (t_token *)e_state->token_list->content;
	while (token->content[e_state->current_pos])
	{
		if (token->content[e_state->current_pos] == '\'')
			tmp = update_state_with_quote(e_state, token, IN_QUOTE, quote_removed);
		else if (token->content[e_state->current_pos] == '\"')
			tmp = update_state_with_quote(e_state, token, IN_DQUOTE, quote_removed);
		else if (e_state->quote_state == NORMAL)
		{
			extracted = ft_substr(token->content, e_state->current_pos, 1);
			tmp = ft_strjoin(quote_removed, extracted);
			free(extracted);
		}
		else
		{
			e_state->current_pos++;
			continue ;
		}
		free(quote_removed);
		quote_removed = tmp;
		e_state->current_pos++;
	}
	q_removed = make_token(quote_removed, 0, ft_strlen(quote_removed),
			token->attr);
	free(quote_removed);
	return (q_removed);
}

t_list	*remove_quote(t_expand_state *e_state)
{
	t_token			*q_removed;
	t_list			*tmp_list;
	t_list			*expanded_list;

	expanded_list = NULL;
	while (e_state->token_list != NULL)
	{
		q_removed = remove_quote_core(e_state);
		ft_lstadd_node(&expanded_list, q_removed);
		tmp_list = e_state->token_list->next;
		// ft_lstdelone(e_state.token_list, free);
		e_state->token_list = tmp_list;
	}
	return (expanded_list);
}

#include "minishell.h"

void	init_expand_state(t_expand_state *e_state)
{
	e_state->start = 0;
	e_state->current_pos = 0;
	e_state->quote_state = NORMAL;
}

void	update_state_with_quote(t_expand_state *e_state,
		t_quote_state quote_state, char **quote_removed)
{
	if (e_state->quote_state == NORMAL)
	{
		e_state->quote_state = quote_state;
		e_state->start = e_state->current_pos;
	}
	else if (e_state->quote_state == quote_state)
	{
		*quote_removed = ft_strjoin(*quote_removed,
				ft_substr(e_state->origin_token->content,
					e_state->start + 1,
					e_state->current_pos - e_state->start - 1));
		e_state->quote_state = NORMAL;
	}
}

void	remove_quote(t_expand_state *e_state)
{
	char	*quote_removed;

	quote_removed = ft_strdup("");
	init_expand_state(e_state);
	while (e_state->origin_token->content[e_state->current_pos])
	{
		if (e_state->origin_token->content[e_state->current_pos] == '\'')
			update_state_with_quote(e_state, IN_QUOTE, &quote_removed);
		else if (e_state->origin_token->content[e_state->current_pos] == '\"')
			update_state_with_quote(e_state, IN_DQUOTE, &quote_removed);
		else if (e_state->quote_state == NORMAL)
			quote_removed = ft_strjoin(quote_removed,
					ft_substr(e_state->origin_token->content,
						e_state->current_pos, 1));
		e_state->current_pos++;
	}
	e_state->origin_token->content = quote_removed;
}

int	expand(t_list *token_list, t_list **expanded_list, t_envs *envs)
{
	t_expand_state	e_state;

	init_expand_state(&e_state);
	while (token_list != NULL)
	{
		e_state.origin_token = (t_token *)malloc(sizeof(t_token));
		e_state.origin_token->attr = ((t_token *)token_list->content)->attr;
		e_state.origin_token->content
			= ((t_token *)token_list->content)->content;
		if ((e_state.origin_token->attr == TK_WORD
				|| e_state.origin_token->attr == TK_DOUBLE_QUOTED)
			&& ft_strchr(e_state.origin_token->content, '$') != NULL)
			e_state.origin_token->content = expand_env_vals(&e_state, envs);
		remove_quote(&e_state);
		if (ft_lstadd_node(expanded_list, e_state.origin_token) == FAIL)
			return (FAIL);
		token_list = token_list->next;
	}
	return (SUCCESS);
}

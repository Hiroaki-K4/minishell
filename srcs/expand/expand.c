#include "minishell.h"

void	init_expand_state(t_expand_state *e_state)
{
	e_state->start = 0;
	e_state->current_pos = 0;
	e_state->quote_state = NORMAL;
}

void	update_state_with_quote(t_expand_state *e_state, t_token *token,
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
				ft_substr(token->content,
					e_state->start + 1,
					e_state->current_pos - e_state->start - 1));
		e_state->quote_state = NORMAL;
	}
}

t_token	*remove_quote(t_expand_state *e_state)
{
	char	*quote_removed;
	t_token	*q_removed;
	t_token	*token;

	q_removed = NULL;
	quote_removed = ft_strdup("");
	init_expand_state(e_state);
	token = (t_token *)e_state->token_list->content;
	while (token->content[e_state->current_pos])
	{
		if (token->content[e_state->current_pos] == '\'')
			update_state_with_quote(e_state, token, IN_QUOTE, &quote_removed);
		else if (token->content[e_state->current_pos] == '\"')
			update_state_with_quote(e_state, token, IN_DQUOTE, &quote_removed);
		else if (e_state->quote_state == NORMAL)
			quote_removed = ft_strjoin(quote_removed,
					ft_substr(token->content,
						e_state->current_pos, 1));
		e_state->current_pos++;
	}
	q_removed = make_token(quote_removed, 0, ft_strlen(quote_removed),
			token->attr);
	return (q_removed);
}

int	expand(t_list *token_list, t_list **expanded_list, t_envs *envs,
	int exit_status)
{
	t_token			*q_removed;
	t_expand_state	e_state;

	init_expand_state(&e_state);
	e_state.token_list = NULL;
	while (token_list != NULL)
	{
		e_state.original_token = (t_token *)malloc(sizeof(t_token));
		e_state.original_token->attr = ((t_token *)token_list->content)->attr;
		e_state.original_token->content
			= ((t_token *)token_list->content)->content;
		if (ft_strchr(e_state.original_token->content, '$') != NULL)
			expand_env_vals(&e_state, envs, exit_status);
		else
			ft_lstadd_node(&(e_state.token_list), e_state.original_token);
		while (e_state.token_list != NULL)
		{
			q_removed = remove_quote(&e_state);
			ft_lstadd_node(expanded_list, q_removed);
			e_state.token_list = e_state.token_list->next;
		}
		token_list = token_list->next;
	}
	return (SUCCESS);
}

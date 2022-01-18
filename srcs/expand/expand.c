#include "minishell.h"

void	init_expand_state(t_expand_state *expand_state)
{
	expand_state->start = 0;
	expand_state->current_pos = 0;
	expand_state->quote_state = NORMAL;
}

void	update_state_with_quote(t_expand_state *expand_state,
		t_quote_state quote_state, char **quote_removed)
{
	if (expand_state->quote_state == NORMAL)
	{
		expand_state->quote_state = quote_state;
		expand_state->start = expand_state->current_pos;
	}
	else if (expand_state->quote_state == quote_state)
	{
		*quote_removed = ft_strjoin(*quote_removed,
				ft_substr(expand_state->expanded_token->content,
					expand_state->start + 1,
					expand_state->current_pos - expand_state->start - 1));
		expand_state->quote_state = NORMAL;
	}
}

void	remove_quote(t_expand_state *expand_state)
{
	char	*quote_removed;

	quote_removed = ft_strdup("");
	init_expand_state(expand_state);
	while (expand_state->expanded_token->content[expand_state->current_pos])
	{
		if (expand_state->expanded_token
			->content[expand_state->current_pos] == '\'')
			update_state_with_quote(expand_state, IN_QUOTE, &quote_removed);
		else if (expand_state->expanded_token
			->content[expand_state->current_pos] == '\"')
			update_state_with_quote(expand_state, IN_DQUOTE, &quote_removed);
		else if (expand_state->quote_state == NORMAL)
			quote_removed = ft_strjoin(quote_removed,
					ft_substr(expand_state->expanded_token->content,
						expand_state->current_pos, 1));
		expand_state->current_pos++;
	}
	expand_state->expanded_token->content = quote_removed;
}

int	expand(t_list *token_list, t_list **expanded_list, t_envs *envs)
{
	t_expand_state	expand_state;

	init_expand_state(&expand_state);
	while (token_list != NULL)
	{
		expand_state.expanded_token = (t_token *)malloc(sizeof(t_token));
		expand_state.expanded_token->attr
			= ((t_token *)token_list->content)->attr;
		expand_state.expanded_token->content
			= ((t_token *)token_list->content)->content;
		if ((expand_state.expanded_token->attr == TK_WORD
				|| expand_state.expanded_token->attr == TK_DOUBLE_QUOTED)
			&& ft_strchr(expand_state.expanded_token->content, '$') != NULL)
			expand_state.expanded_token->content
				= expand_env_vals(&expand_state, envs);
		remove_quote(&expand_state);
		if (ft_lstadd_node(expanded_list, expand_state.expanded_token) == FAIL)
			return (FAIL);
		token_list = token_list->next;
	}
	return (SUCCESS);
}

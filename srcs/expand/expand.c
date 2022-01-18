#include "minishell.h"

void	init_expand_state(t_expand_state *expand_state)
{
	expand_state->start = 0;
	expand_state->current_pos = 0;
	expand_state->quote_state = NORMAL;
}

void	update_state_with_quote(t_expand_state *expand_state,
		t_quote_state quote_state, char **without_quote)
{
	if (expand_state->quote_state == NORMAL)
	{
		expand_state->quote_state = quote_state;
		expand_state->start = expand_state->current_pos;
	}
	else if (expand_state->quote_state == quote_state)
	{
		*without_quote = ft_strjoin(*without_quote,
				ft_substr(expand_state->expanded_token->content,
					expand_state->start + 1,
					expand_state->current_pos - expand_state->start - 1));
		expand_state->quote_state = NORMAL;
	}
}

void	remove_quote(t_expand_state *expand_state)
{
	char	*without_quote;

	without_quote = ft_strdup("");
	init_expand_state(expand_state);
	while (expand_state->expanded_token->content[expand_state->current_pos])
	{
		if (expand_state->expanded_token
			->content[expand_state->current_pos] == '\'')
			update_state_with_quote(expand_state, IN_QUOTE, &without_quote);
		else if (expand_state->expanded_token
			->content[expand_state->current_pos] == '\"')
			update_state_with_quote(expand_state, IN_DQUOTE, &without_quote);
		else if (expand_state->quote_state == NORMAL)
			without_quote = ft_strjoin(without_quote,
					ft_substr(expand_state->expanded_token->content,
						expand_state->current_pos, 1));
		expand_state->current_pos++;
	}
	expand_state->expanded_token->content = without_quote;
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

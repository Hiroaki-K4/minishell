#include "minishell.h"

char	*remove_quote(t_expand_state *e_state, t_token *token, char *progress)
{
	char	*quote_removed;
	char	*extracted;

	extracted = ft_substr(token->content, e_state->start + 1,
			e_state->current_pos - e_state->start - 1);
	quote_removed = ft_strjoin(progress, extracted);
	free(extracted);
	e_state->quote_state = NORMAL;
	return (quote_removed);
}

void	change_q_state(t_expand_state *e_state, char quote)
{
	if (quote == '\'')
		e_state->quote_state = IN_QUOTE;
	else if (quote == '\"')
		e_state->quote_state = IN_DQUOTE;
	e_state->start = e_state->current_pos;
	e_state->current_pos++;
}

char	*get_word_in_quote(t_token *token, t_expand_state *e_state, char *word)
{
	char	*tmp;

	if (token->content[e_state->current_pos] == '\''
		&& e_state->quote_state == IN_QUOTE)
		tmp = remove_quote(e_state, token, word);
	else if (token->content[e_state->current_pos] == '\"'
		&& e_state->quote_state == IN_DQUOTE)
		tmp = remove_quote(e_state, token, word);
	else
		tmp = ft_strdup(word);
	return (tmp);
}

char	*read_current_char(char *content, size_t current_pos, char *q_removed)
{
	char	*current_char;
	char	*str;

	current_char = ft_substr(content, current_pos, 1);
	str = ft_strjoin(q_removed, current_char);
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

t_list	*check_quote(t_expand_state *e_state)
{
	t_token			*q_removed;
	t_token			*token;
	t_list			*tmp_list;
	t_list			*expanded_list;
	char			*quote_removed;

	expanded_list = NULL;
	while (e_state->token_list != NULL)
	{
		token = (t_token *)e_state->token_list->content;
		init_expand_state(e_state);
		quote_removed = get_q_removed(e_state, token, ft_strdup(""), NULL);
		q_removed = make_token(quote_removed, 0, ft_strlen(quote_removed),
				token->attr);
		ft_lstadd_node(&expanded_list, q_removed);
		tmp_list = e_state->token_list->next;
		e_state->token_list = tmp_list;
	}
	return (expanded_list);
}

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

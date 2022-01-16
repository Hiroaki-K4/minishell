#include "minishell.h"

void	update_quote_state(t_quote_state *state, char c)
{
	if (c == '\'' && *state == NORMAL)
		*state = IN_QUOTE;
	else if (c == '\"' && *state == NORMAL)
		*state = IN_DQUOTE;
	else if ((c == '\'' && *state == IN_QUOTE)
		|| (c == '\"' && *state == IN_DQUOTE))
		*state = NORMAL;
}

int	read_line(char *line, t_list **token_list, t_tokenize_state *tokenize_state)
{
	if (is_metacharacter_without_token_kind(line[tokenize_state->current_pos])
		&& tokenize_state->quote_state == NORMAL)
		return (separate_by_no_kind_sep_word(line, token_list, tokenize_state));
	else if ((is_separating_word(line, tokenize_state->current_pos)
			&& tokenize_state->quote_state == NORMAL))
		return (separate_by_sep_word(line, token_list, tokenize_state));
	else if (line[tokenize_state->current_pos + 1] == '\0')
		return (separate_by_null_char(line, token_list, tokenize_state));
	else
		tokenize_state->current_pos++;
	return (SUCCESS);
}

void	init_tokenize_state(t_tokenize_state	*tokenize_state)
{
	tokenize_state->trim_start = 0;
	tokenize_state->current_pos = 0;
	tokenize_state->quote_state = NORMAL;
	tokenize_state->token_kind = TK_WORD;
}

int	tokenize(char *line, t_list **token_list)
{
	t_tokenize_state	tokenize_state;

	init_tokenize_state(&tokenize_state);
	while (line[tokenize_state.current_pos]
		&& tokenize_state.current_pos < ft_strlen(line))
	{
		update_quote_state(&tokenize_state.quote_state,
			line[tokenize_state.current_pos]);
		if (read_line(line, token_list, &tokenize_state) == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}

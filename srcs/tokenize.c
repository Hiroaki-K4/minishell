#include "minishell.h"

void	check_quote_state(t_quote_state *state, char c)
{
	if (c == '\'' && *state == NORMAL)
		*state = IN_QUOTE;
	else if (c == '\"' && *state == NORMAL)
		*state = IN_DQUOTE;
	else if ((c == '\'' && *state == IN_QUOTE) || (c == '\"' && *state == IN_DQUOTE))
		*state = NORMAL;
}

int	read_line(char *line, t_list **token_list, t_tokenizer *tokenizer)
{
	if ((line[tokenizer->current_pos] == ' ' || line[tokenizer->current_pos] == '\t' || line[tokenizer->current_pos] == '\n') && tokenizer->quote_state == NORMAL)
		return (separate_by_no_kind_sep_word(line, token_list, tokenizer));
	else if ((is_separating_word(line, tokenizer->current_pos) && tokenizer->quote_state == NORMAL))
		return (separate_by_sep_word(line, token_list, tokenizer));
	else if (line[tokenizer->current_pos + 1] == '\0')
		return (separate_by_null_char(line, token_list, tokenizer));
	else
		tokenizer->current_pos++;
	return (SUCCESS);
}

int	tokenize(char *line, t_list **token_list)
{
	t_tokenizer	tokenizer;

	tokenizer.trim_start = 0;
	tokenizer.current_pos = 0;
	tokenizer.quote_state = NORMAL;
	tokenizer.token_kind = TK_WORD;
	while (line[tokenizer.current_pos] && tokenizer.current_pos < ft_strlen(line))
	{
		check_quote_state(&tokenizer.quote_state, line[tokenizer.current_pos]);
		if (read_line(line, token_list, &tokenizer) == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}

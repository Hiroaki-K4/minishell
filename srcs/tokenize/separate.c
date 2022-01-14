#include "minishell.h"

int	separate_by_no_kind_sep_word(char *line, t_list **token_list,
	t_tokenizer *tokenizer)
{
	size_t	i;
	t_token	*new_token;

	i = 0;
	tokenizer->token_kind = get_token_kind(line, tokenizer->current_pos, &i);
	if (tokenizer->trim_start != tokenizer->current_pos)
	{
		new_token = make_token(line, tokenizer->trim_start,
				tokenizer->current_pos - tokenizer->trim_start,
				tokenizer->token_kind);
		if (ft_lstadd_node(token_list, new_token) == FAIL)
			return (FAIL);
	}
	tokenizer->current_pos += i;
	tokenizer->trim_start = tokenizer->current_pos;
	return (SUCCESS);
}

int	separate_by_sep_word(char *line, t_list **token_list,
	t_tokenizer *tokenizer)
{
	size_t	i;
	t_token	*new_token;

	i = 0;
	tokenizer->token_kind = get_token_kind(line, tokenizer->current_pos, &i);
	if (tokenizer->trim_start != tokenizer->current_pos)
	{
		new_token = make_token(line, tokenizer->trim_start,
				tokenizer->current_pos - tokenizer->trim_start,
				get_token_kind_about_quote(tokenizer->quote_state,
					line[tokenizer->current_pos - 1]));
		if (ft_lstadd_node(token_list, new_token) == FAIL)
			return (FAIL);
		tokenizer->trim_start = tokenizer->current_pos;
	}
	new_token = make_token(line, tokenizer->trim_start,
			tokenizer->current_pos + i - tokenizer->trim_start,
			tokenizer->token_kind);
	if (ft_lstadd_node(token_list, new_token) == FAIL)
		return (FAIL);
	tokenizer->current_pos += i;
	tokenizer->trim_start = tokenizer->current_pos;
	return (SUCCESS);
}

int	separate_by_null_char(char *line, t_list **token_list,
	t_tokenizer *tokenizer)
{
	size_t	i;
	t_token	*new_token;

	i = 0;
	tokenizer->current_pos++;
	new_token = make_token(line, tokenizer->trim_start,
			tokenizer->current_pos - tokenizer->trim_start,
			get_token_kind_about_quote(tokenizer->quote_state,
				line[tokenizer->current_pos - 1]));
	if (ft_lstadd_node(token_list, new_token) == FAIL)
		return (FAIL);
	tokenizer->trim_start = tokenizer->current_pos + i;
	return (SUCCESS);
}

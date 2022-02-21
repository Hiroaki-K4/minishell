/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:25:51 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/20 21:25:52 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	read_line(char *line, t_list **token_list, t_tokenize_state *t_state)
{
	if (is_metacharacter_without_token_kind(line[t_state->current_pos])
		&& t_state->quote_state == NORMAL)
		return (separate_by_no_kind_sep_word(line, token_list, t_state));
	else if ((is_separating_word(line, t_state->current_pos)
			&& t_state->quote_state == NORMAL))
		return (separate_by_sep_word(line, token_list, t_state));
	else if (line[t_state->current_pos + 1] == '\0')
		return (separate_by_null_char(line, token_list, t_state));
	else
		t_state->current_pos++;
	return (SUCCESS);
}

void	init_tokenize_state(t_tokenize_state *t_state)
{
	t_state->trim_start = 0;
	t_state->current_pos = 0;
	t_state->quote_state = NORMAL;
	t_state->token_kind = TK_WORD;
}

int	tokenize(char *line, t_list **token_list)
{
	t_tokenize_state	t_state;

	init_tokenize_state(&t_state);
	while (line[t_state.current_pos]
		&& t_state.current_pos < ft_strlen(line))
	{
		update_quote_state(&t_state.quote_state,
			line[t_state.current_pos]);
		if (read_line(line, token_list, &t_state) == FAIL)
			return (FAIL);
	}
	if (t_state.quote_state != NORMAL)
	{
		printf("minishell: syntax error: unexpected end of file\n");
		return (FAIL);
	}
	return (SUCCESS);
}

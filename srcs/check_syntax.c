#include "minishell.h"

int	is_pipe_or_semicolon(int attr)
{
	return (attr == TK_PIPE || attr == TK_SEMICOLON);
}

int	is_redirect(int attr)
{
	return (attr == TK_REDIRECT_IN || attr == TK_REDIRECT_OUT
		|| attr == TK_REDIRECT_MULTI || attr == TK_REDIRECT_APPEND);
}

int	check_syntax(t_list *token_list)
{
	t_token	*content;
	t_token	*next_content;

	content = (t_token *)token_list->content;
	if (token_list->prev == NULL && is_pipe_or_semicolon(content->attr))
		return (FAIL);
	while (token_list->next != NULL)
	{
		content = (t_token *)token_list->content;
		next_content = (t_token *)token_list->next->content;
		if ((is_pipe_or_semicolon(content->attr)
				&& is_pipe_or_semicolon(next_content->attr))
			|| (is_redirect(content->attr)
				&& is_pipe_or_semicolon(next_content->attr))
			|| (is_redirect(content->attr) && is_redirect(next_content->attr)))
			return (FAIL);
		token_list = token_list->next;
	}
	content = (t_token *)token_list->content;
	if (is_redirect(content->attr))
		return (FAIL);
	return (SUCCESS);
}

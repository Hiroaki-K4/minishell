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
	int			first_flag;
	t_command	*content;
	t_command	*next_content;

	first_flag = TRUE;
	while (token_list->next != NULL)
	{
		content = (t_command *)token_list->content;
		next_content = (t_command *)token_list->next->content;
		if ((first_flag == TRUE && is_pipe_or_semicolon(content->attr))
			|| (is_pipe_or_semicolon(content->attr)
				&& is_pipe_or_semicolon(next_content->attr))
			|| (is_redirect(content->attr)
				&& is_pipe_or_semicolon(next_content->attr))
			|| (is_redirect(content->attr) && is_redirect(next_content->attr)))
			return (FAIL);
		if (first_flag == TRUE)
			first_flag = FALSE;
		token_list = token_list->next;
	}
	content = (t_command *)token_list->content;
	if (is_redirect(content->attr))
		return (FAIL);
	return (SUCCESS);
}

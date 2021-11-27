#include "minishell.h"

int		check_syntax(t_list *token_list)
{
	int			first_flag;
	t_command	*content;
	t_command	*next_content;
	
	first_flag = 1;
	while (token_list->next != NULL)
	{
		content = (t_command *)token_list->content;
		next_content = (t_command *)token_list->next->content;
		if (first_flag == 1 && content->attr == TK_PIPE)
			return (-1);
		else if (content->attr == TK_PIPE && next_content->attr == TK_PIPE)
			return (-1);
		if (first_flag == 1)
			first_flag = 0;
		token_list = token_list->next;
	}
	return (0);
}

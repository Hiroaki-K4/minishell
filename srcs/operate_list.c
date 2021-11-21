#include "minishell.h"

t_command	*init_content()
{
	t_command	*content;

	if (!(content = (t_command *)malloc(sizeof(t_command))))
		return (NULL);
	content->content = "";
	content->attr = START;
	content->next = NULL;
	return (content);
}

t_command	*get_last_list(t_command *list)
{
	while (list->next != NULL)
		list = list->next;
	return (list);
}
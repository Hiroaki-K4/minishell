#include "minishell.h"

void	init_command_list(t_command **command_list)
{
	(*command_list)->context = "";
	(*command_list)->attr = START;
	(*command_list)->next = NULL;
}

t_command	*get_last_list(t_command *list)
{
	while (list->next != NULL)
		list = list->next;
	return (list);
}
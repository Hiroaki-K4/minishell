#include "minishell.h"

t_command	*init_command_list()
{
	t_command	*command_list;

	if (!(command_list = (t_command *)malloc(sizeof(t_command))))
		return (NULL);
	command_list->context = "";
	command_list->attr = START;
	command_list->next = NULL;
	return (command_list);
}

t_command	*get_last_list(t_command *list)
{
	while (list->next != NULL)
		list = list->next;
	return (list);
}
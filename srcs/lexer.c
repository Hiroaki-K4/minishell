#include "minishell.h"


char	*delete_space(char *command)
{
	char	*trimed1;
	char	*trimed2;

	trimed1 = ft_strtrim(command, " ");
	trimed2 = ft_strtrim(trimed1, "\t");
	free(trimed1);
	return (trimed2);
}

void	init_command_list(t_command *command_list)
{
	command_list->context = "";
	command_list->attr = START;
	command_list->next = NULL;
}

t_command	*get_last_list(t_command *list)
{
	while (list->next != NULL)
		list = list->next;
	return (list);
}

int	store_token(char *trimed, t_command **command_list, int pos, int i)
{
	t_command	*new;
	t_command	*split;
	t_command	*last;

	if (trimed[i] == '|' || trimed[i] == ' ' || trimed[i + 1] == '\0')
	{
		new = (t_command *)malloc(sizeof(t_command));
		if (!new)
			return (-1);
		split = (t_command *)malloc(sizeof(t_command));
		if (!split)
			return (-1);
		last = get_last_list(*command_list);
		printf("pos: %d len: %d\n", pos, i - pos);
		if (trimed[i + 1] == '\0')
			new->context = ft_substr(trimed, pos, i + 1 - pos);
		else
			new->context = ft_substr(trimed, pos, i - pos);
		printf("new: %s\n", (char *)new->context);
		new->next = split;
		split->context = ft_substr(trimed, i, 1);
		split->next = NULL;
		last->next = new;
		return (i + 1);
	}
	return (pos);
}

void	tokenize(char *trimed, t_command *command_list)
{
	int	i;
	int	pos;

	i = 0;
	pos = 0;
	while (trimed[i])
	{
		pos = store_token(trimed, &command_list, pos, i);
		i++;
	}
	while (command_list != NULL)
	{
		printf("context: %s\n", (char *)command_list->context);
		command_list = command_list->next;
	}
}

int	lexer(char *line)
{
	char *trimed;
	t_command	command_list;

	trimed = delete_space(line);
	init_command_list(&command_list);
	tokenize(trimed, &command_list);
	// printf("context: %s attr: %d\n", (char *)command_list.context, command_list.attr);
	// printf("lexer: %s\n", trimed);
	return (0);
}
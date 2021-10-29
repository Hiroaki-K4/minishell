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
	t_command	*last;

	last = get_last_list(*command);
	new->context = ft_substr(trimed, pos, i + 1 - pos);
	new->next = NULL;
	last->next = new;
	// printf("tmp: %s\n", (char *)tmp->context);
	// printf("command: %s\n", (char *)command_list->context);
	// if (command_list->next == NULL)
	// {
	// 	printf("pos: %d i: %d\n", pos, i);
	// 	command_list->context = ft_substr(trimed, pos, i + 1 - pos);
	// 	printf("context: %s\n", (char *)command_list->context);
	// }
	return (i + 1);
}

void	tokenize(char *trimed, t_command *command_list)
{
	int	i;
	int	pos;

	i = 0;
	pos = 0;
	while (trimed[i])
	{
		if (trimed[i] == '|')
			pos = store_token(trimed, &command_list, pos, i);
		else
			pos = store_token(trimed, &command_list, pos, i);
		// printf("%c\n", trimed[i]);
		i++;
	}
	printf("command_")
}

int	lexer(char *line)
{
	char *trimed;
	t_command	command_list;

	trimed = delete_space(line);
	init_command_list(&command_list);
	tokenize(trimed, &command_list);
	printf("context: %s attr: %d\n", (char *)command_list.context, command_list.attr);
	printf("lexer: %s\n", trimed);
	return (0);
}
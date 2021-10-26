#include "minishell.h"


char	*delete_space(char *command)
{
	char	*trimed1;
	char	*trimed2;

	trimed1 = ft_strtrim(command, " ");
	trimed2 = ft_strtrim(trimed1, "\f");
	free(trimed1);
	trimed1 = ft_strtrim(trimed2, "\n");
	free(trimed2);
	trimed2 = ft_strtrim(trimed1, "\r");
	free(trimed1);
	trimed1 = ft_strtrim(trimed2, "\t");
	free(trimed2);
	trimed2 = ft_strtrim(trimed1, "\v");
	return (trimed2);
}

void	init_command_list(t_command *command_list)
{
	command_list->context = "";
	command_list->attr = START;
	command_list->next = NULL;
}

void	tokenize(char *trimed, t_command *command_list)
{
	(void)command_list;
	printf("trimed: %s\n", trimed);
}

int	lexer(char *line)
{
	char *trimed;
	t_command	command_list;

	trimed = delete_space(line);
	init_command_list(&command_list);
	tokenize(trimed, &command_list);
	printf("context: %s attr: %d\n", (char *)command_list.context, command_list.attr);
	free(line);
	printf("lexer: %s\n", trimed);
	return (0);
}
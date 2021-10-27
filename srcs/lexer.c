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

int	store_token(char *trimed, t_command *command_list, int pos, int i)
{
	if (command_list->next == NULL)
	{
		printf("pos: %d i: %d\n", pos, i);
		command_list->context = ft_substr(trimed, pos, i - pos);
		printf("context: %s\n", command_list->context);
	}
	else
	{
		while (command_list->next != NULL)
		{

		}
	}

	printf("substr: %s\n", ft_substr(trimed, 0, 10));
	return (i + 1);
}

void	tokenize(char *trimed, t_command *command_list)
{
	int	i;
	int	pos;

	(void)command_list;
	printf("trimed: %s\n", trimed);
	i = 0;
	pos = 0;
	while (trimed[i])
	{
		if (trimed[i] == '|')
			pos = store_token(trimed, command_list, pos, i);
		else
			pos = store_token(trimed, command_list, pos, i);
		printf("%c\n", trimed[i]);
		i++;
	}
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
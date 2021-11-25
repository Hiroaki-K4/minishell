#include "minishell.h"

void	parse(t_list **command_list)
{
	(void)command_list;
}

void	output_result(void *content)
{
	t_command	*command;

	command = (t_command *)content;
	printf("content: %s attr: %d\n", (char *)command->content, command->attr);
}

int	preprocess(char *line)
{
	char		*trimed;
	t_list		*command_list;

	command_list = NULL;
	trimed = ft_strtrim(line, " \t");
	tokenize(trimed, &command_list);
	parse(&command_list);
	ft_lstiter(command_list, output_result);
	return (SUCCESS);
}

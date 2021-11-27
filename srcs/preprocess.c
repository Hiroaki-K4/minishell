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

int		check_syntax(t_list *command_list)
{
	int	first_flag;
	t_command	*content;
	t_command	*next_content;
	
	first_flag = 1;
	command_list = command_list->next;
	while (command_list->next != NULL)
	{
		content = (t_command *)command_list->content;
		next_content = (t_command *)command_list->next->content;
		if (first_flag == 1 && content->attr == PIPE)
			return (-1);
		else if (content->attr == PIPE && next_content->attr == PIPE)
			printf("double pipe\n");
			// return (-1);
		if (first_flag == 1)
			first_flag = 0;
		command_list = command_list->next;
		printf("ok\n");
	}
	return (0);
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

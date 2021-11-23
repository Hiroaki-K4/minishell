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

void	tokenize(char *trimed, t_list **command_list)
{
	int		i;
	int		pos;

	i = 0;
	pos = 0;
	while (trimed[i])
	{
		if (trimed[i] == '|' || trimed[i] == ' ' || trimed[i] == '<' || trimed[i] == '>' || trimed[i] == '\'' || trimed[i] == '\"' || trimed[i + 1] == '\0')
		{
			if (trimed[i] == ' ' && trimed[i - 1] == ' ')
				pos = i + 1;
			else
			{
				pos = store_token(trimed, command_list, pos, &i);
				if ((trimed[i] == '<' && trimed[i + 1] == '<') || (trimed[i] == '>' && trimed[i + 1] == '>'))
					i++;
			}
		}
		i++;
	}
}

void	parser(t_list **command_list)
{
	(void)command_list;
}

void	output_result(void *content)
{
	t_command *command;

	command = (t_command *)content;
	printf("content: %s attr: %d\n", (char *)command->content, command->attr);
}

int	preprocess(char *line)
{
	char		*trimed;
	t_list		*command_list;
	t_command	*content;

	trimed = delete_space(line);
	content = init_content();
	command_list = ft_lstnew(content);
	if (!command_list)
		return (-1);
	tokenize(trimed, &command_list);
	parser(&command_list);
	ft_lstiter(command_list, output_result);
	return (0);
}

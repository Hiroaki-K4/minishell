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

int	store_token(char *trimed, t_command **command_list, int pos, int *i)
{
	t_command	*new;
	t_command	*split;
	t_command	*last;
	int			new_pos;

	// if (trimed[*i] == '|' || trimed[*i] == ' ' || trimed[*i] == '<' || trimed[*i] == '>' || trimed[*i] == '\'' || trimed[*i] == '\"' || trimed[*i + 1] == '\0')
	// {
	new_pos = *i + 1;
	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
		return (-1);
	split = (t_command *)malloc(sizeof(t_command));
	if (!split)
		return (-1);
	last = get_last_list(*command_list);
	printf("pos: %d len: %d word: %c\n", pos, *i - pos, trimed[*i]);
	if (trimed[*i + 1] == '\0')
	{
		new->context = ft_substr(trimed, pos, *i + 1 - pos);
		split->context = ft_substr(trimed, *i + 1, 1);
	}
	else
	{
		new->context = ft_substr(trimed, pos, *i - pos);
		new->attr = STR;
		if ((trimed[*i] == '<' && trimed[*i + 1] == '<') || (trimed[*i] == '>' && trimed[*i + 1] == '>'))
		{
			new_pos = *i + 2;
			split->context = ft_substr(trimed, *i, 2);
		}
		else
			split->context = ft_substr(trimed, *i, 1);
		if (trimed[*i] == ' ')
		{
			if (trimed[*i - 1] == ' ')
			{
				free(new);
				free(split);
				return (*i + 1);
			}
			split->attr = SPACES;
		}
		else if (trimed[*i] == '|')
			split->attr = PIPE;
		else if (trimed[*i] == '<' && trimed[*i + 1] == '<')
			split->attr = REDIRECT_MULTI;
		else if (trimed[*i] == '>' && trimed[*i + 1] == '>')
			split->attr = REDIRECT_APPEND;
		else if (trimed[*i] == '<')
			split->attr = REDIRECT_IN;
		else if (trimed[*i] == '>')
			split->attr = REDIRECT_OUT;
		else if (trimed[*i] == '\'')
			split->attr = SQUOTE;
		else if (trimed[*i] == '\"')
			split->attr = DQUOTE;
	}
	split->next = NULL;
	if (i - pos == 0)
	{
		free(new);
		last->next = split;
	}
	else
	{
		new->next = split;
		last->next = new;
	}
	if ((trimed[*i] == '<' && trimed[*i + 1] == '<') || (trimed[*i] == '>' && trimed[*i + 1] == '>'))
		(*i)++;
	return (new_pos);
	// }
	// return (pos);
}

void	tokenize(char *trimed, t_command *command_list)
{
	int	i;
	int	pos;

	i = 0;
	pos = 0;
	while (trimed[i])
	{
		if (trimed[*i] == '|' || trimed[*i] == ' ' || trimed[*i] == '<' || trimed[*i] == '>' || trimed[*i] == '\'' || trimed[*i] == '\"' || trimed[*i + 1] == '\0')
			pos = store_token(trimed, &command_list, pos, &i);
		i++;
	}
	while (command_list != NULL)
	{
		printf("context: %s attr: %d\n", (char *)command_list->context, command_list->attr);
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
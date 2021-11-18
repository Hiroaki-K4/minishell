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

t_command	*decide_attr(t_command *split, char word)
{
	if (word == ' ')
		split->attr = SPACES;
	else if (word == '|')
		split->attr = PIPE;
	else if (word == '<' && word == '<')
		split->attr = REDIRECT_MULTI;
	else if (word == '>' && word == '>')
		split->attr = REDIRECT_APPEND;
	else if (word == '<')
		split->attr = REDIRECT_IN;
	else if (word == '>')
		split->attr = REDIRECT_OUT;
	else if (word == '\'')
		split->attr = SQUOTE;
	else if (word == '\"')
		split->attr = DQUOTE;
	split->next = NULL;
	return (split);
}

int	store_token(char *trimed, t_command *last, int pos, int *i)
{
	t_command	*new;
	t_command	*split;
	int			new_pos;

	new_pos = *i + 1;
	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
		return (-1);
	split = (t_command *)malloc(sizeof(t_command));
	if (!split)
		return (-1);
	if (trimed[*i + 1] == '\0')
	{
		new->context = ft_substr(trimed, pos, *i + 1 - pos);
		new = decide_attr(new, trimed[*i]);
		split->context = ft_substr(trimed, *i + 1, 1);
		split->attr = END;
		split->next = NULL;
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
		split = decide_attr(split, trimed[*i]);
	}
	if (ft_strncmp((char *)new->context, "\0", 1) == 0 && new->attr == STR)
	{
		free(new);
		last->next = split;
	}
	else
	{
		new->next = split;
		last->next = new;
	}
	return (new_pos);
}

void	tokenize(char *trimed, t_command **command_list)
{
	int	i;
	int	pos;
	t_command	*last;

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
				last = get_last_list(*command_list);
				pos = store_token(trimed, last, pos, &i);
				if ((trimed[i] == '<' && trimed[i + 1] == '<') || (trimed[i] == '>' && trimed[i + 1] == '>'))
					i++;
			}
		}
		i++;
	}
	// while (command_list != NULL)
	// {
	// 	printf("context: %s attr: %d\n", (char *)command_list.context, command_list.attr);
	// 	command_list = command_list.next;
	// }
}

int	lexer(char *line)
{
	char *trimed;
	t_command	*command_list;

	trimed = delete_space(line);
	init_command_list(&command_list);
	tokenize(trimed, &command_list);
	// while (&command_list != NULL)
	// {
		// printf("context: %s attr: %d\n", (char *)command_list.context, command_list.attr);
		// command_list = command_list.next;
	// }
	return (0);
}
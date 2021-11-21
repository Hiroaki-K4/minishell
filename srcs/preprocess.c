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

t_command	*decide_attr(t_command *token, char *trimed, int *i)
{
	if (trimed[*i] == ' ')
		token->attr = SPACES;
	else if (trimed[*i] == '|')
		token->attr = PIPE;
	else if (trimed[*i] == '<' && trimed[*i + 1] == '<')
		token->attr = REDIRECT_MULTI;
	else if (trimed[*i] == '>' && trimed[*i + 1] == '>')
		token->attr = REDIRECT_APPEND;
	else if (trimed[*i] == '<')
		token->attr = REDIRECT_IN;
	else if (trimed[*i] == '>')
		token->attr = REDIRECT_OUT;
	else if (trimed[*i] == '\'')
		token->attr = SQUOTE;
	else if (trimed[*i] == '\"')
		token->attr = DQUOTE;
	else
		token->attr = STR;
	token->next = NULL;
	return (token);
}

void	clear_list(void *content)
{
	(void)content;
}

int	store_token(char *trimed, t_list **command_list, int pos, int *i)
{
	int			new_pos;
	t_list		*new_list;
	t_list		*split_list;
	t_command	*new;
	t_command	*split;

	new_pos = *i + 1;
	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
		return (-1);
	split = (t_command *)malloc(sizeof(t_command));
	if (!split)
		return (-1);
	if (trimed[*i + 1] == '\0')
	{
		new->content = ft_substr(trimed, pos, *i + 1 - pos);
		new = decide_attr(new, trimed, i);
		split->content = ft_substr(trimed, *i + 1, 1);
		split->attr = END;
		split->next = NULL;
	}
	else
	{
		new->content = ft_substr(trimed, pos, *i - pos);
		new->attr = STR;
		if ((trimed[*i] == '<' && trimed[*i + 1] == '<') || (trimed[*i] == '>' && trimed[*i + 1] == '>'))
		{
			new_pos = *i + 2;
			split->content = ft_substr(trimed, *i, 2);
		}
		else
			split->content = ft_substr(trimed, *i, 1);
		split = decide_attr(split, trimed, i);
	}
	if (ft_strncmp((char *)new->content, "\0", 1) == 0 && new->attr == STR)
	{
		free(new);
		new_list = ft_lstnew(split);
		ft_lstadd_back(command_list, new_list);
	}
	else
	{
		new_list = ft_lstnew(new);
		ft_lstadd_back(command_list, new_list);
		split_list = ft_lstnew(split);
		ft_lstadd_back(command_list, split_list);
	}
	return (new_pos);
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

	command = content;
	printf("content: %s attr: %d\n", (char *)command->content, command->attr);
}

int	preprocess(char *line)
{
	char		*trimed;
	t_list		*command_list;
	t_command	*content;

	trimed = delete_space(line);
	content = init_command_list();
	command_list = ft_lstnew(content);
	if (!command_list)
		return (-1);
	tokenize(trimed, &command_list);
	parser(&command_list);
	ft_lstiter(command_list, output_result);
	return (0);
}

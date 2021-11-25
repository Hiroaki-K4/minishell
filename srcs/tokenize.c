#include "minishell.h"

int	is_metacharacter(char c)
{
	return (c == ' ' || c == '\n' || c == '\t'
		|| c == '|' || c == '<' || c == '>');
}

int	is_quotes(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_separating_character(char c)
{
	return (is_metacharacter(c) || is_quotes(c));
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

int	store_token(char *line, t_list **command_list, int pos, int *i)
{
	int			new_pos;
	t_list		*new_list;
	t_list		*split_list;
	t_command	*new;
	t_command	*split;

	new_pos = *i + 1;
	new = (t_command *)malloc(sizeof(t_command));
	if (new == NULL)
		return (FAIL);
	split = (t_command *)malloc(sizeof(t_command));
	if (split == NULL)
		return (FAIL);
	if (line[*i + 1] == '\0')
	{
		new->content = ft_substr(line, pos, *i + 1 - pos);
		new = decide_attr(new, line, i);
		split->content = ft_substr(line, *i + 1, 1);
		split->attr = END;
		split->next = NULL;
	}
	else
	{
		new->content = ft_substr(line, pos, *i - pos);
		new->attr = STR;
		if ((line[*i] == '<' && line[*i + 1] == '<')
			|| (line[*i] == '>' && line[*i + 1] == '>'))
		{
			new_pos = *i + 2;
			split->content = ft_substr(line, *i, 2);
		}
		else
			split->content = ft_substr(line, *i, 1);
		split = decide_attr(split, line, i);
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

void	tokenize(char *line, t_list **command_list)
{
	int		i;
	int		pos;

	i = 0;
	pos = 0;
	while (line[i])
	{
		if (is_metacharacter(line[i]) || line[i + 1] == '\0')
		{
			if (line[i] == ' ' && line[i - 1] == ' ')
				pos = i + 1;
			else
			{
				pos = store_token(line, command_list, pos, &i);
				if ((line[i] == '<' && line[i + 1] == '<')
					|| (line[i] == '>' && line[i + 1] == '>'))
					i++;
			}
		}
		i++;
	}
}

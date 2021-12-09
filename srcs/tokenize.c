#include "minishell.h"

int	is_metacharacter(char c)
{
	return (c == ' ' || c == '\n' || c == '\t'
		|| c == '|' || c == '<' || c == '>'
		|| c == '(' || c == ')' || c == ';');
}

int	is_quotes(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_separating_character(char c)
{
	return (is_metacharacter(c) || is_quotes(c));
}

t_token_kind	decide_attr(char *line, int pos)
{
	t_token_kind	token_kind;

	if (line[pos] == ' ')
		token_kind = TK_SPACE;
	else if (line[pos] == '|')
		token_kind = TK_PIPE;
	else if (line[pos] == '<' && line[pos + 1] == '<')
		token_kind = TK_REDIRECT_MULTI;
	else if (line[pos] == '>' && line[pos + 1] == '>')
		token_kind = TK_REDIRECT_APPEND;
	else if (line[pos] == '<')
		token_kind = TK_REDIRECT_IN;
	else if (line[pos] == '>')
		token_kind = TK_REDIRECT_OUT;
	else if (line[pos] == '\'')
		token_kind = TK_SINGLE_QUOTE;
	else if (line[pos] == '\"')
		token_kind = TK_DOUBLE_QUOTE;
	else if (line[pos] == ';')
		token_kind = TK_SEMICOLON;
	else
		token_kind = TK_WORD;
	return (token_kind);
}

int	store_operator(char *line, t_list **token_list, size_t pos)
{
	size_t		i;
	t_list		*new_node;
	t_command	*new_token;

	i = 1;
	new_token = (t_command *)malloc(sizeof(t_command));
	if (new_token == NULL)
		return (FAIL);
	if ((line[pos] == '<' && line[pos + i] == '<')
		|| (line[pos] == '>' && line[pos + i] == '>'))
		new_token->content = ft_substr(line, pos, i + 1);
	else
	{
		new_token->content = ft_substr(line, pos, i);
		i = 0;
	}
	new_token->attr = decide_attr(line, pos);
	new_token->next = NULL;
	new_node = ft_lstnew(new_token);
	if (new_node == NULL)
		return (FAIL);
	ft_lstadd_back(token_list, new_node);
	return (pos + i);
}

int	store_word(char *line, t_list **token_list, size_t pos)
{
	size_t		i;
	t_list		*new_node;
	t_command	*new_token;

	i = 0;
	new_token = (t_command *)malloc(sizeof(t_command));
	if (new_token == NULL)
		return (FAIL);
	while (line[pos + i])
	{
		if (is_metacharacter(line[pos + i]))
			break ;
		i++;
	}
	new_token->content = ft_substr(line, pos, i);
	new_token->attr = decide_attr(line, pos);
	new_token->next = NULL;
	new_node = ft_lstnew(new_token);
	if (new_node == NULL)
		return (FAIL);
	ft_lstadd_back(token_list, new_node);
	return (pos + i);
}

void	tokenize(char *line, t_list **token_list)
{
	size_t	pos;

	pos = 0;
	while (line[pos] && pos < ft_strlen(line))
	{
		if (is_separating_character(line[pos]))
		{
			if ((line[pos] != ' ' && line[pos] != '\t' && line[pos] != '\n'))
				pos = store_operator(line, token_list, pos);
		}
		else
		{
			pos = store_word(line, token_list, pos);
		}
		pos++;
	}
}

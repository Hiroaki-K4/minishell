#include "minishell.h"

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
	else if (line[pos] == ';')
		token_kind = TK_SEMICOLON;
	else
		token_kind = TK_WORD;
	return (token_kind);
}

t_command	*make_token(t_command *token, char *line, size_t pos, int i)
{
	token->content = ft_substr(line, pos, i);
	if (token->attr != TK_SINGLE_QUOTED && token->attr != TK_DOUBLE_QUOTED)
		token->attr = decide_attr(line, pos);
	token->next = NULL;
	return (token);
}

t_command	*check_quote(char *line, size_t pos, size_t *i,
	t_command *token)
{
	while (line[pos + *i] != line[pos] && line[pos + *i])
		(*i)++;
	if (line[pos + *i] == line[pos] && line[pos] == '\'')
		token->attr = TK_SINGLE_QUOTED;
	else if (line[pos + *i] == line[pos] && line[pos] == '\"')
		token->attr = TK_DOUBLE_QUOTED;
	else
		token->attr = TK_WORD;
	token = make_token(token, line, pos, *i + 1);
	return (token);
}

int	ft_lstadd_node(t_list **token_list, t_command *new_token)
{
	t_list	*new_node;

	new_node = ft_lstnew(new_token);
	if (new_node == NULL)
		return (FAIL);
	ft_lstadd_back(token_list, new_node);
	return (SUCCESS);
}

int	store_operator(char *line, t_list **token_list, size_t pos)
{
	size_t		i;
	t_command	*new_token;

	i = 1;
	new_token = (t_command *)malloc(sizeof(t_command));
	if (new_token == NULL)
		return (FAIL);
	if (line[pos] == '\'' || line[pos] == '\"')
	{
		new_token = check_quote(line, pos, &i, new_token);
		if (ft_lstadd_node(token_list, new_token) == FAIL)
			return (FAIL);
		return (pos + i);
	}
	else if ((line[pos] == '<' && line[pos + i] == '<')
		|| (line[pos] == '>' && line[pos + i] == '>'))
		new_token = make_token(new_token, line, pos, i + 1);
	else
	{
		new_token = make_token(new_token, line, pos, i);
		i = 0;
	}
	if (ft_lstadd_node(token_list, new_token) == FAIL)
		return (FAIL);
	return (pos + i);
}

int	store_word(char *line, t_list **token_list, size_t pos)
{
	size_t		i;
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
	new_token = make_token(new_token, line, pos, i);
	if (ft_lstadd_node(token_list, new_token) == FAIL)
		return (FAIL);
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

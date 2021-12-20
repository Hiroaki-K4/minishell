#include "minishell.h"

t_token	*make_token(char *line, size_t pos, size_t len, t_token_kind attr)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (new_token == NULL)
		return (NULL);
	new_token->content = ft_substr(line, pos, len);
	new_token->attr = attr;
	return (new_token);
}

int	ft_lstadd_node(t_list **token_list, t_token *new_token)
{
	t_list	*new_node;

	new_node = ft_lstnew(new_token);
	if (new_node == NULL)
		return (FAIL);
	ft_lstadd_back(token_list, new_node);
	return (SUCCESS);
}

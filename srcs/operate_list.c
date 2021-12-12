#include "minishell.h"

t_command	*make_token(t_command *token, char *line, size_t pos, int i)
{
	token->content = ft_substr(line, pos, i);
	if (token->attr != TK_SINGLE_QUOTED && token->attr != TK_DOUBLE_QUOTED)
		token->attr = decide_attr(line, pos);
	token->next = NULL;
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

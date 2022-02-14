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

void	ft_lstdelone_all(t_list *lst, void (*del)(void*))
{
	t_token	*content;

	if (lst)
	{
		content = (t_token *)lst->content;
		(*del)(content->content);
		(*del)(lst->content);
		free(lst);
	}
}

void	ft_lstclear_all(t_list **lst, void (*del)(void*))
{
	t_list	*tmp;
	t_token	*content;

	if (!del)
		return ;
	while (*lst)
	{
		content = (*lst)->content;
		(*del)(content->content);
		(*del)(content);
		tmp = *lst;
		*lst = tmp->next;
		free(tmp);
	}
	*lst = NULL;
}

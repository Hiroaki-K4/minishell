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

int	ft_lstadd_token(t_list **token_list, t_token *new_token)
{
	t_list	*new_list;

	new_list = ft_lstnew(new_token);
	if (new_list == NULL)
		return (FAIL);
	ft_lstadd_back(token_list, new_list);
	return (SUCCESS);
}

int	ft_lstadd_word(t_list **lst, char *new_word)
{
	char	*tmp;
	t_list	*last_list;
	t_token	*new_token;
	t_token	*last_token;

	if (!*lst)
	{
		new_token = make_token(new_word, 0, ft_strlen(new_word), TK_WORD);
		if (ft_lstadd_token(lst, new_token) == FAIL)
			return (FAIL);
		return (SUCCESS);
	}
	else
	{
		last_list = ft_lstlast(*lst);
		last_token = (t_token *)last_list->content;
		tmp = ft_strjoin(last_token->content, new_word);
		free(last_token->content);
		last_token->content = tmp;
		if (last_token->content == NULL)
			return (FAIL);
	}
	return (SUCCESS);
}

void	ft_lstadd_last(t_list **lst, t_list *new)
{
	char	*tmp;
	t_token	*new_token;
	t_token	*last_token;
	t_list	*last_lst;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last_lst = ft_lstlast(*lst);
	new_token = (t_token *)new->content;
	last_token = (t_token *)last_lst->content;
	tmp = ft_strjoin(last_token->content, new_token->content);
	free(last_token->content);
	last_token->content = tmp;
	if (new->next != NULL)
	{
		last_lst->next = new->next;
		new->next->prev = last_lst;
	}
	else
		last_lst->next = NULL;
	ft_lstdelone_all(new, free);
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

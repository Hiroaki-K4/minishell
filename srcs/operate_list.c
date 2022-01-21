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

int	ft_lstadd_word(t_list **lst, char *new_word)
{
	t_list	*last_lst;
	t_token	*new_token;
	t_token	*last_token;

	if (!*lst)
	{
		new_token = make_token(new_word, 0, ft_strlen(new_word), TK_WORD);
		if (ft_lstadd_node(lst, new_token) == FAIL)
			return (FAIL);
		return (SUCCESS);
	}
	else
	{
		last_lst = ft_lstlast(*lst);
		last_token = (t_token *)last_lst->content;
		last_token->content = ft_strjoin(last_token->content, new_word);
		if (last_token->content == NULL)
			return (FAIL);
	}
	return (SUCCESS);
}

void	ft_lstadd_last(t_list **lst, t_list *new)
{
	char	*new_word;
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
	new_word = new_token->content;
	last_token = (t_token *)last_lst->content;
	last_token->content = ft_strjoin(last_token->content, new_word);
	if (new->next != NULL)
	{
		new->next->prev = last_lst;
		last_lst->next = new->next;
	}
	else
		last_lst->next = NULL;
}

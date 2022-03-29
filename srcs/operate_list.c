/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operate_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 21:27:10 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/27 20:18:42 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*make_token(char *line, size_t pos, size_t len, t_token_kind attr)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (new_token == NULL)
		return (NULL);
	new_token->content = ft_substr(line, pos, len);
	if (!new_token->content)
		return (NULL);
	new_token->attr = attr;
	new_token->q_removed = FALSE;
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

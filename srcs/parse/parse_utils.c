/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:48:39 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/21 22:48:40 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	consume_token(t_list **token_list, t_token_kind kind)
{
	if (!(*token_list))
		return (FALSE);
	if (((t_token *)(*token_list)->content)->attr == kind)
	{
		*token_list = (*token_list)->next;
		return (TRUE);
	}
	return (FALSE);
}

t_node	*new_node(t_node *lhs, t_node *rhs, t_node_kind attr)
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	if (new_node == NULL)
		return (NULL);
	new_node->lhs = lhs;
	new_node->rhs = rhs;
	new_node->attr = attr;
	new_node->tokens = NULL;
	new_node->is_furthest_left = FALSE;
	new_node->is_furthest_right = FALSE;
	new_node->is_top = FALSE;
	return (new_node);
}

int	is_command_token(t_list **token_list)
{
	return (consume_token(token_list, TK_WORD)
		|| consume_token(token_list, TK_IO_NUMBER)
		|| consume_token(token_list, TK_REDIRECT_IN)
		|| consume_token(token_list, TK_REDIRECT_OUT)
		|| consume_token(token_list, TK_REDIRECT_DLESS)
		|| consume_token(token_list, TK_REDIRECT_DGREAT));
}

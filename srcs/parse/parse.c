/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychida <ychida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 19:43:33 by ychida            #+#    #+#             */
/*   Updated: 2022/02/05 14:26:43 by ychida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_command(t_list **token_list)
{
	t_list	*head;
	t_node	*node;

	head = *token_list;
	node = new_node(NULL, NULL, ND_COMMAND);
	if ((*token_list)->prev == NULL)
		node->is_furthest_left = TRUE;
	while (is_command_token(token_list))
		;
	if (*token_list)
		(*token_list)->prev->next = NULL;
	else
		node->is_furthest_right = TRUE;
	node->tokens = head;
	return (node);
}

t_node	*parse_pipe_sequence(t_list **token_list)
{
	t_node	*node;

	node = parse_command(token_list);
	while (TRUE)
	{
		if (consume_token(token_list, TK_PIPE))
			node = new_node(node, parse_command(token_list), ND_PIPE);
		else
			return (node);
	}
}

t_node	*parse(t_list **token_list)
{
	t_node	*node;

	node = parse_pipe_sequence(token_list);
	node->is_top = TRUE;
	return (node);
}

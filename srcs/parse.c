/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 19:43:33 by ychida            #+#    #+#             */
/*   Updated: 2021/12/17 09:31:16 by hkubo            ###   ########.fr       */
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
	return (new_node);
}

t_node	*parse_command(t_list **token_list)
{
	t_list	*head;
	t_node	*node;

	head = *token_list;
	node = new_node(NULL, NULL, ND_COMMAND);
	while (consume_token(token_list, TK_WORD)
		|| consume_token(token_list, TK_REDIRECT_IN)
		|| consume_token(token_list, TK_REDIRECT_OUT))
		;
	if (*token_list)
		(*token_list)->prev->next = NULL;
	node->tokens = head;
	return (node);
}

t_node	*parse_pipe(t_list **token_list)
{
	t_node	*node;

	node = parse_command(token_list);
	if (consume_token(token_list, TK_PIPE))
		node = new_node(node, parse_command(token_list), ND_PIPE);
	return (node);
}

t_node	*parse_semicolon(t_list **token_list)
{
	t_node	*node;

	node = parse_pipe(token_list);
	if (consume_token(token_list, TK_SEMICOLON))
		node = new_node(node, parse_pipe(token_list), ND_SEMICOLON);
	return (node);
}

void	print_ast(t_node *node)
{
	if (node->lhs)
		print_ast(node->lhs);
	if (node->rhs)
		print_ast(node->rhs);
	if (node->tokens)
	{
		printf("tokens\n");
		ft_lstiter(node->tokens, output_result);
		printf("\n");
	}
}

t_node	*parse(t_list **token_list)
{
	t_node	*node;

	node = parse_semicolon(token_list);
	// print_ast(node);
	return (node);
}

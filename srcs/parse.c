/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychida <ychida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 19:43:33 by ychida            #+#    #+#             */
/*   Updated: 2021/12/13 22:59:49 by ychida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	consume_token(t_list **command_list, t_token_kind kind)
{
	if (!(*command_list))
		return (FALSE);
	if (((t_command *)(*command_list)->content)->attr == kind)
	{
		*command_list = (*command_list)->next;
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
	new_node->commands = NULL;
	return (new_node);
}

t_node	*parse_command(t_list **command_list)
{
	t_list	*head;
	t_node	*node;

	head = *command_list;
	node = new_node(NULL, NULL, ND_COMMAND);
	while (consume_token(command_list, TK_WORD)
		|| consume_token(command_list, TK_REDIRECT_IN)
		|| consume_token(command_list, TK_REDIRECT_OUT))
		;
	if (*command_list)
		(*command_list)->prev->next = NULL;
	node->commands = head;
	return (node);
}

t_node	*parse_pipe(t_list **command_list)
{
	t_node	*node;

	node = parse_command(command_list);
	if (consume_token(command_list, TK_PIPE))
		node = new_node(node, parse_command(command_list), ND_PIPE);
	return (node);
}

t_node	*parse_semicolon(t_list **command_list)
{
	t_node	*node;

	node = parse_pipe(command_list);
	if (consume_token(command_list, TK_SEMICOLON))
		node = new_node(node, parse_pipe(command_list), ND_SEMICOLON);
	return (node);
}

void	print_ast(t_node *node)
{
	if (node->lhs)
		print_ast(node->lhs);
	if (node->rhs)
		print_ast(node->rhs);
	if (node->commands)
	{
		printf("commands\n");
		ft_lstiter(node->commands, output_result);
		printf("\n");
	}
}

t_node	*parse(t_list **command_list)
{
	t_node	*node;

	node = parse_semicolon(command_list);
	// print_ast(node);
	return (node);
}

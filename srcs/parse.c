/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychida <ychida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 19:43:33 by ychida            #+#    #+#             */
/*   Updated: 2021/12/20 16:12:32 by ychida           ###   ########.fr       */
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
	return (new_node);
}

int	is_command_token(t_list **token_list)
{
	return (consume_token(token_list, TK_WORD)
		|| consume_token(token_list, TK_SINGLE_QUOTED)
		|| consume_token(token_list, TK_DOUBLE_QUOTED)
		|| consume_token(token_list, TK_IO_NUMBER)
		|| consume_token(token_list, TK_REDIRECT_IN)
		|| consume_token(token_list, TK_REDIRECT_OUT)
		|| consume_token(token_list, TK_REDIRECT_DLESS)
		|| consume_token(token_list, TK_REDIRECT_DGREAT));
}

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

int g_node_count;

void	print_ast(t_node *node, FILE *fp)
{
	int	node_id;

	g_node_count++;
	node_id = g_node_count;
	if (node->lhs)
	{
		fputs("  ", fp);
		putc(node_id + '0', fp);
		fputs(" -> ", fp);
		putc(g_node_count + 1 + '0', fp);
		fputs(";\n", fp);
		print_ast(node->lhs, fp);
	}
	if (node->rhs)
	{
		fputs("  ", fp);
		putc(node_id + '0', fp);
		fputs(" -> ", fp);
		putc(g_node_count + 1 + '0', fp);
		fputs(";\n", fp);
		print_ast(node->rhs, fp);
	}
	printf("tokens\n");
	if (node->tokens)
		ft_lstiter(node->tokens, output_result);
	else
		printf("(null)\n");
	printf("\n");

	fputs("  ", fp);
	putc(node_id + '0', fp);
	fputs(" [\n    label = \"", fp);
	if (node->attr == ND_PIPE)
		fputs("pipe", fp);
	else if (node->attr == ND_COMMAND)
	{
		fputs("command", fp);
		fputs("\n", fp);
		while (node->tokens)
		{
			fputs(((t_token *)(node->tokens->content))->content, fp);
			fputs(" ", fp);
			node->tokens = node->tokens->next;
		}
	}
	fputs("\",\n  ];\n", fp);
}

t_node	*parse(t_list **token_list)
{
	FILE	*fp;
	t_node	*node;

	node = parse_pipe_sequence(token_list);
	if ((fp = fopen("ast.dot", "w+")) == NULL)
	{
		printf("failed open file\n");
		exit(EXIT_FAILURE);
	}
	fputs("digraph graph_name {\n", fp);
	print_ast(node, fp);
	fputs("}", fp);
	fclose(fp);
	return (node);
}

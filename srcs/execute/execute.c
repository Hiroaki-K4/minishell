/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:48:05 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/21 22:48:06 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_ast(t_node *ast)
{
	if (ast->lhs != NULL)
		free_ast(ast->lhs);
	if (ast->rhs != NULL)
		free_ast(ast->rhs);
	ft_lstclear_all(&ast->tokens, free);
	free(ast);
}

int	execute(t_node *ast, t_global_state *state)
{
	int	ret;

	if (ast->attr == ND_PIPE)
		ret = execute_pipe(ast, state);
	else
		ret = execute_commands(ast, NULL, state);
	return (ret);
}

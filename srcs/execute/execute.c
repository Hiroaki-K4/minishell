#include "minishell.h"

static void	free_ast(t_node *ast)
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
	free_ast(ast);
	return (ret);
}

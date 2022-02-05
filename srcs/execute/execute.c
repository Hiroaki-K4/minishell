#include "minishell.h"

int	execute(t_node *ast, t_global_state *state)
{
	if (ast->attr == ND_PIPE)
		return (execute_pipe(ast, state));
	return (execute_commands(ast, NULL, state));
}

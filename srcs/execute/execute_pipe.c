#include "minishell.h"

static void	execute_pipe_pattern1(
	t_node *ast,
	t_global_state *state,
	int fildes[2]
)
{
	execute_pipe(ast->lhs, state);
	if (pipe(fildes) == FAIL)
		exit_with_error("pipe error");
	execute_commands(ast->lhs->rhs, fildes, state);
	if (ast->is_top)
	{
		state->old_pipes[0] = fildes[0];
		state->old_pipes[1] = fildes[1];
		execute_commands(ast->rhs, fildes, state);
	}
	state->old_pipes[0] = fildes[0];
	state->old_pipes[1] = fildes[1];
}

static void	execute_pipe_pattern2(
	t_node *ast,
	t_global_state *state,
	int fildes[2]
)
{
	if (pipe(fildes) == FAIL)
		exit_with_error("pipe error");
	execute_commands(ast->lhs, fildes, state);
	if (ast->is_top)
		execute_commands(ast->rhs, fildes, state);
	state->old_pipes[0] = fildes[0];
	state->old_pipes[1] = fildes[1];
}

int	execute_pipe(t_node *ast, t_global_state *state)
{
	int		fildes[2];

	if (ast->lhs->attr == ND_PIPE)
		execute_pipe_pattern1(ast, state, fildes);
	else if (ast->lhs->attr == ND_COMMAND)
		execute_pipe_pattern2(ast, state, fildes);
	return (SUCCESS);
}

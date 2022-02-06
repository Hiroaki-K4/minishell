#include "minishell.h"

void	set_initial_handlers(t_global_state *state)
{
	set_sigaction(&state->sa_sigint, sigint_handler);
	set_sigaction(&state->sa_sigquit, SIG_IGN);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
}

void	set_child_handlers(t_global_state *state)
{
	set_sigaction(&state->sa_sigint, SIG_DFL);
	set_sigaction(&state->sa_sigquit, SIG_DFL);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
}

void	set_parent_handlers(t_global_state *state)
{
	set_sigaction(&state->sa_sigint, SIG_IGN);
	set_sigaction(&state->sa_sigquit, SIG_IGN);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
}

#include "minishell.h"

void	set_handlers(t_global_state *state)
{
	set_sigaction(&state->sa_sigint, sigint_handler);
	set_sigaction(&state->sa_sigquit, SIG_IGN);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
}

void	set_handlers2(t_global_state *state)
{
	set_sigaction(&state->sa_sigint, nop_handler);
	set_sigaction(&state->sa_sigquit, nop_handler);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
}

void	set_handlers3(t_global_state *state)
{
	set_sigaction(&state->sa_sigint, sigint_handler2);
	set_sigaction(&state->sa_sigquit, sigquit_handler2);
	if (sigaction(SIGINT, &state->sa_sigint, NULL) < 0
		|| sigaction(SIGQUIT, &state->sa_sigquit, NULL) < 0)
		exit_with_error("sigaction error");
}

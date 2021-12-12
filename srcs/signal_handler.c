#include "minishell.h"

void	sigint_handler(int sig)
{
	printf("sig: %d\n", sig);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	printf("sig: %d\n", sig);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

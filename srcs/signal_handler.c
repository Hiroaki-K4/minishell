#include "minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	init_sigaction(
	struct sigaction *sa_sigint,
	struct sigaction *sa_sigquit
)
{
	ft_bzero(sa_sigint, sizeof(*sa_sigint));
	ft_bzero(sa_sigquit, sizeof(*sa_sigquit));
	sa_sigint->sa_handler = sigint_handler;
	sa_sigquit->sa_handler = sigquit_handler;
}

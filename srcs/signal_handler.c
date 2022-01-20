#include "minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;

	rl_on_new_line();
	rl_redisplay();
	write(1, "  \n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	(void)sig;

	rl_on_new_line();
	rl_redisplay();
	write(1, "  \b\b", 4);
	rl_replace_line(rl_line_buffer, 0);
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

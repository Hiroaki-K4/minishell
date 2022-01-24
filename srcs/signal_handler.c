#include "minishell.h"

void	set_sigaction(struct sigaction *sigaction, void (*handler)(int))
{
	ft_bzero(sigaction, sizeof(*sigaction));
	sigaction->sa_handler = handler;
}

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

void	sigint_handler2(int sig)
{
	(void)sig;

	write(1, "\n", 1);
}

void	sigquit_handler2(int sig)
{
	(void)sig;

	write(1, "Quit: 3\n", ft_strlen("Quit: 3\n"));
}

void	nop_handler(int sig)
{
	(void)sig;
}

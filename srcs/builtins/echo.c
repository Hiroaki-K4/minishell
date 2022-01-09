#include "minishell.h"

int	ft_echo(char **argv, t_global_state *state)
{
	int		has_n_option;
	size_t	i;

	(void)state;
	if (argv[1] != NULL && !ft_strncmp(argv[1], "-n", 3))
	{
		has_n_option = TRUE;
		i = 2;
	}
	else
	{
		has_n_option = FALSE;
		i = 1;
	}
	while (argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		i++;
		if (argv[i] != NULL)
			write(1, " ", 1);
	}
	if (!has_n_option)
		write(1, "\n", 1);
	return (SUCCESS);
}

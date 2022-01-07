#include "minishell.h"

int	ft_export(char **argv)
{
	if (argv[1] == NULL)
	{
		extern char	**environ;
		char	**env;

		env = environ;
		while (*env)
		{
			const char *val;

			val = *env;
			if (val)
				printf("%s\n", val);
			env++;
		}
	}
	else
	{
		printf("two\n");
	}
	return (SUCCESS);
}

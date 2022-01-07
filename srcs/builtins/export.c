#include "minishell.h"

int	ft_export(char **argv)
{
	size_t	i;
	size_t	j;
	char	*val;
	char	*word;

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
		i = 1;
		while (argv[i])
		{
			printf("i: %ld\n", i);
			j = 0;
			if (ft_strchr(argv[i], '=') == NULL)
			{
				val = argv[i];
				word = NULL;
				printf("val: %s word: %s\n", val, word);
				if (setenv(val, word, 1) == -1)
					printf("FAILED\n");
			}
			else
			{
				while (argv[i][j])
				{
					if (argv[i][j] == '=')
					{
						if (j == 0)
							return (FAIL);
						else
						{
							val = ft_substr(argv[i], 0, j);
							word = ft_substr(argv[i], j + 1, ft_strlen(argv[i]) - j);
							if (word == NULL)
								word = ft_strdup("");
							printf("val: %s word: %s\n", val, word);
							if (setenv(val, word, 1) == -1)
								printf("FAILED\n");
						}
					}
					j++;
				}
			}

			i++;
		}
	}
	return (SUCCESS);
}

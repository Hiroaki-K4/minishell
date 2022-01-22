#include "minishell.h"

#define BUF_SIZE 1024

int	ft_pwd(char **argv, t_envs *envs, int *exit_status)
{
	int		count;
	char	*buf;
	char	*fp;

	(void)argv;
	(void)envs;
	(void)exit_status;
	count = 1;
	while (TRUE)
	{
		buf = (char *)malloc(sizeof(char) * (BUF_SIZE * count));
		if (buf == NULL)
			return (FAIL);
		fp = getcwd(buf, BUF_SIZE * count);
		if (fp != NULL)
			break ;
		free(buf);
		count++;
	}
	printf("%s\n", fp);
	return (SUCCESS);
}

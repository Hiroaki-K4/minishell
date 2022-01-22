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
	buf = NULL;
	fp = getcwd(buf, 0);
	if (fp == NULL)
	{
		fp = get_env("PWD", envs);
		printf("%s\n", fp);
	}
	else
	{
		set_env("PWD", fp, &envs);
		printf("%s\n", fp);
		free(fp);
	}
	return (SUCCESS);
}

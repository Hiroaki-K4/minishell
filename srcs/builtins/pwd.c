#include "minishell.h"

#define BUF_SIZE 1024

int	ft_pwd(char **argv, t_envs *envs)
{
	char	*buf;
	char	*fp;

	(void)argv;
	(void)envs;
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

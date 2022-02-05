#include "minishell.h"

#define BUF_SIZE 1024

int	ft_pwd(char **argv, t_envs *envs)
{
	char	*buf;

	(void)argv;
	(void)envs;
	buf = NULL;
	getcwd(buf, 0);
	if (buf == NULL)
		buf = get_env("PWD", envs);
	else
		set_env("PWD", buf, &envs);
	printf("%s\n", buf);
	free(buf);
	return (SUCCESS);
}

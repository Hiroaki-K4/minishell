#include "minishell.h"

int	is_special_builtin_command(char **argv, t_envs **envs)
{
	int		ret;
	size_t	i;
	char *builtins[] = {"cd", "export", "unset", "exit", NULL};
	int (*builtin_funcs[])(char **, t_envs **) = {ft_cd, ft_export, ft_unset, ft_exit};

	i = 0;
	if (argv[0] == NULL)
		return (FALSE);
	while (builtins[i])
	{
		if (!ft_strncmp(argv[0], builtins[i], ft_strlen(builtins[i]) + 1))
		{
			ret = builtin_funcs[i](argv, envs);
			(void)ret;  // TODO: use ret value
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

int	is_builtin_command(char **argv, t_envs *envs)
{
	int		ret;
	size_t	i;
	char *builtins[] = {"echo", "pwd", "env", NULL};
	int (*builtin_funcs[])(char **, t_envs *) = {ft_echo, ft_pwd, ft_env};

	i = 0;
	while (builtins[i])
	{
		if (!ft_strncmp(argv[0], builtins[i], ft_strlen(builtins[i]) + 1))
		{
			ret = builtin_funcs[i](argv, envs);
			(void)ret;  // TODO: use ret value
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

#include "minishell.h"

static int	construct_redirects(t_list **tokens, t_global_state *state)
{
	size_t	i;

	i = 0;
	while (i < state->redirect_num - 1)
		i++;
	state->redirects[i] = (t_redirect *)malloc(sizeof(t_redirect));
	if (state->redirects[i] == NULL)
		return (FAIL);
	init_redirect(state->redirects[i]);
	if (((t_token *)((*tokens)->content))->attr == TK_IO_NUMBER)
	{
		state->redirects[i]->redirect_fd
			= ft_atoi(((t_token *)((*tokens)->content))->content);
		*tokens = (*tokens)->next;
	}
	if (set_redirect(tokens, state->redirects[i], state->envs) == FAIL)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline`", 2);
		return (FAIL);
	}
	return (SUCCESS);
}

static size_t	consume_words(t_list **tokens, char **argv, size_t start)
{
	size_t	i;

	i = 0;
	while (TRUE)
	{
		if (*tokens == NULL
			|| is_redirect_token((t_token *)((*tokens)->content)))
			break ;
		argv[start + i] = (((t_token *)((*tokens)->content))->content);
		*tokens = (*tokens)->next;
		i++;
	}
	return (i);
}

char	**construct_argv(t_list *tokens, t_global_state *state)
{
	size_t		idx;
	char		**argv;

	idx = 0;
	argv = (char **)malloc(sizeof(char *) * (ft_lstsize(tokens) + 1));
	if (argv == NULL)
		return (NULL);
	while (TRUE)
	{
		idx += consume_words(&tokens, argv, idx);
		if (tokens == NULL)
			break ;
		state->redirect_num++;
		if (construct_redirects(&tokens, state) == SUCCESS)
			continue ;
		while (*argv)
		{
			free(*argv);
			argv++;
		}
		free(argv);
		return (NULL);
	}
	argv[idx] = NULL;
	return (argv);
}

#include "minishell.h"

int	construct_redirects(t_list **tokens, t_global_state *state)
{
	int	i;

	i = 0;
	while (i < state->redirect_num - 1)
		i++;
	state->redirects[i] = (t_redirect *)malloc(sizeof(t_redirect));
	if (state->redirects[i] == NULL)
		return (FAIL);
	init_redirect(state->redirects[i]);
	set_redirect(tokens, state->redirects[i], state->envs);
	return (SUCCESS);
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
		while (TRUE)
		{
			if (tokens == NULL
				|| is_redirect_token((t_token *)(tokens->content)))
				break ;
			argv[idx] = (((t_token *)(tokens->content))->content);
			tokens = tokens->next;
			idx++;
		}
		if (tokens == NULL)
			break ;
		state->redirect_num++;
		if (construct_redirects(&tokens, state) == FAIL)
		{
			while (*argv)
			{
				free(*argv);
				argv++;
			}
			free(argv);
			return (NULL);
		}
	}
	argv[idx] = NULL;
	return (argv);
}

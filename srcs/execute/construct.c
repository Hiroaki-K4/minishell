#include "minishell.h"

int	construct_redirects(t_list *tokens, t_global_state *state)
{
	int			i;
	t_redirect	*tmp;

	i = 0;
	state->redirect_num++;
	tmp = (t_redirect *)malloc(sizeof(t_redirect) * state->redirect_num);
	if (tmp == NULL)
		return (FAIL);
	while (i < state->redirect_num - 1)
	{
		tmp[i] = state->redirects[i];
		i++;
	}
	state->redirects = tmp;
	init_redirect(&state->redirects[state->redirect_num - 1]);
	set_redirect(&tokens, &state->redirects[state->redirect_num - 1], state->envs);
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
		if (construct_redirects(tokens, state) == FAIL)
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

#include "minishell.h"

static int	is_redirect_token(t_token *token)
{
	return (token->attr == TK_IO_NUMBER
		|| token->attr == TK_REDIRECT_IN
		|| token->attr == TK_REDIRECT_OUT
		|| token->attr == TK_REDIRECT_DGREAT
		|| token->attr == TK_REDIRECT_DLESS);
}

static void	init_redirect(t_redirect *redirect)
{
	redirect->redirect_fd = -1;
	redirect->file_fd = -1;
	redirect->here_delimiter = NULL;
	redirect->here_document = NULL;
}

static int	construct_redirects(t_list **tokens, t_global_state *state)
{
	size_t	i;

	i = state->redirect_num - 1;
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
	return (set_redirect(tokens, state->redirects[i], state->envs));
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
		argv[start + i] = ft_strdup(((t_token *)((*tokens)->content))->content);
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
	ft_bzero(argv, sizeof(char *) * (ft_lstsize(tokens) + 1));
	while (TRUE)
	{
		idx += consume_words(&tokens, argv, idx);
		if (tokens == NULL)
			break ;
		state->redirect_num++;
		if (construct_redirects(&tokens, state) == SUCCESS)
			continue ;
		idx = 0;
		while (argv[idx])
		{
			free(argv[idx]);
			idx++;
		}
		free(argv);
		return (NULL);
	}
	argv[idx] = NULL;
	return (argv);
}

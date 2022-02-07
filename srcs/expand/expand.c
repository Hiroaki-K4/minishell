#include "minishell.h"

void	init_expand_state(t_expand_state *e_state)
{
	e_state->start = 0;
	e_state->current_pos = 0;
	e_state->quote_state = NORMAL;
}

t_expand_state	*update_e_state(t_expand_state *e_state, t_list *token_list,
	t_envs *envs, int exit_status)
{
	char	*empty;

	while (token_list != NULL)
	{
		e_state->original_token = (t_token *)token_list->content;
		if (ft_strchr(e_state->original_token->content, '$') != NULL)
		{
			empty = ft_strdup("");
			ft_lstadd_node(&(e_state->token_list), make_token(empty,
					0, 0, TK_WORD));
			free(empty);
			if (expand_env_vals(e_state, envs, exit_status) == FAIL)
				return (NULL);
		}
		else
			ft_lstadd_node(&(e_state->token_list), e_state->original_token);
		token_list = token_list->next;
	}
	return (e_state);
}

t_expand_state	*make_e_state(t_list *token_list, t_envs *envs, int exit_status)
{
	t_expand_state	*e_state;

	e_state = (t_expand_state *)malloc(sizeof(t_expand_state));
	if (!e_state)
		return (NULL);
	init_expand_state(e_state);
	e_state->token_list = NULL;
	e_state = update_e_state(e_state, token_list, envs, exit_status);
	return (e_state);
}

int	is_content_empty(t_list *list, char *word)
{
	t_token	*token;

	token = (t_token *)list->content;
	if (ft_strncmp(token->content, word, ft_strlen(token->content) + 1) == 0)
		return (TRUE);
	return (FALSE);
}

int	expand(t_list *token_list, t_list **expanded_list, t_envs *envs,
	int exit_status)
{
	t_expand_state	*e_state;
	t_list			*last_lst;

	e_state = make_e_state(token_list, envs, exit_status);
	if (!e_state)
		return (FAIL);
	last_lst = ft_lstlast(e_state->token_list);
	if (is_content_empty(last_lst, ""))
	{
		last_lst->prev->next = NULL;
		ft_lstdelone(last_lst, free);
	}
	*expanded_list = check_quote(e_state);
	free(e_state);
	return (SUCCESS);
}

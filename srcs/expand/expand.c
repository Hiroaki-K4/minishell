#include "minishell.h"

int	expand(t_list *token_list, t_list **expanded_list, t_envs *envs,
	int exit_status)
{
	t_expand_state	*e_state;

	e_state = make_e_state(token_list, envs, exit_status);
	if (!e_state)
		return (FAIL);
	*expanded_list = check_quote(e_state);
	if (!*expanded_list)
		return (FAIL);
	free(e_state);
	return (SUCCESS);
}

#include "minishell.h"

char	*get_name_after_dollar(t_expand_state *expand_state, size_t start)
{
	while (expand_state->expanded_token->content[expand_state->current_pos]
		&& expand_state->expanded_token
		->content[expand_state->current_pos] != ' '
		&& expand_state->expanded_token
		->content[expand_state->current_pos] != '$'
		&& expand_state->expanded_token
		->content[expand_state->current_pos] != '\''
		&& expand_state->expanded_token
		->content[expand_state->current_pos] != '\"'
	)
	{
		expand_state->current_pos++;
		if (expand_state->expanded_token->content[expand_state->current_pos - 1]
			== '?')
		{
			// TODO: Correspond $?
			printf("$? appeared\n");
			continue ;
		}
	}
	if (expand_state->current_pos == start)
		return (ft_strdup("$"));
	return (ft_substr(expand_state->expanded_token->content,
			start, expand_state->current_pos - start));
}

void	check_diff_between_start_and_curernt_pos(
	t_expand_state *expand_state,
	char **expanded
)
{
	if (expand_state->start != expand_state->current_pos)
		*expanded = ft_strjoin(*expanded,
				ft_substr(expand_state->expanded_token->content,
					expand_state->start,
					expand_state->current_pos - expand_state->start));
}

char	*expand_env_vals(t_expand_state *expand_state, t_envs *envs)
{
	char	*name;
	char	*value;
	char	*expanded;

	init_expand_state(expand_state);
	expanded = ft_strdup("");
	while (expand_state->expanded_token->content[expand_state->current_pos])
	{
		if (expand_state->expanded_token
			->content[expand_state->current_pos] == '$')
		{
			check_diff_between_start_and_curernt_pos(expand_state, &expanded);
			expand_state->current_pos++;
			name = get_name_after_dollar(expand_state, expand_state->current_pos);
			if (ft_strncmp(name, "$", ft_strlen(name) + 1) == 0)
				expanded = ft_strjoin(expanded, name);
			else
			{
				value = get_env(name, envs);
				if (value != NULL)
					expanded = ft_strjoin(expanded, value);
			}
			expand_state->start = expand_state->current_pos;
			continue ;
		}
		expand_state->current_pos++;
	}
	check_diff_between_start_and_curernt_pos(expand_state, &expanded);
	return (expanded);
}

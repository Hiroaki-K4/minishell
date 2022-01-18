#include "minishell.h"

char	*get_env_from_extracted_word(t_expand_state *expand_state,
			char *env, size_t start, t_envs *envs)
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
	env = get_env(ft_substr(expand_state->expanded_token->content, start,
				expand_state->current_pos - start), envs);
	return (env);
}

void	check_diff_between_start_and_curernt_pos(t_expand_state *expand_state,
	char **expanded)
{
	if (expand_state->start != expand_state->current_pos)
		*expanded = ft_strjoin(*expanded,
				ft_substr(expand_state->expanded_token->content,
					expand_state->start,
					expand_state->current_pos - expand_state->start));
}

char	*expand_env_vals(t_expand_state *expand_state, t_envs *envs)
{
	char	*env;
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
			env = get_env_from_extracted_word(expand_state, env,
					expand_state->current_pos, envs);
			if (env != NULL)
				expanded = ft_strjoin(expanded, env);
			expand_state->start = expand_state->current_pos;
			continue ;
		}
		expand_state->current_pos++;
	}
	check_diff_between_start_and_curernt_pos(expand_state, &expanded);
	return (expanded);
}

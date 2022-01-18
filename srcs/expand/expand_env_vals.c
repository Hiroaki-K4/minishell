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

char	*expand_env_vals(t_expand_state *expand_state, t_envs *envs)
{
	char	*env;
	char	*expanded;

	expand_state->current_pos = 0;
	expand_state->start = 0;
	expanded = ft_strdup("");
	while (expand_state->expanded_token->content[expand_state->current_pos])
	{
		if (expand_state->expanded_token->content[expand_state->current_pos]
			== '$')
		{
			if (expand_state->start != expand_state->current_pos)
				expanded = ft_strjoin(expanded,
						ft_substr(expand_state->expanded_token->content,
							expand_state->start,
							expand_state->current_pos - expand_state->start));
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
	if (expand_state->start < expand_state->current_pos)
		expanded = ft_strjoin(expanded,
				ft_substr(expand_state->expanded_token->content,
					expand_state->start,
					expand_state->current_pos - expand_state->start));
	return (expanded);
}

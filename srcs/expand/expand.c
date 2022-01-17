#include "minishell.h"

char	*get_env_from_extracted_word(t_expand_state *expand_state, char *env, int start, t_envs *envs)
{
	while (expand_state->expanded_token->content[expand_state->current_pos]
		&& expand_state->expanded_token->content[expand_state->current_pos] != ' '
		&& expand_state->expanded_token->content[expand_state->current_pos] != '$'
		&& expand_state->expanded_token->content[expand_state->current_pos] != '\''
		&& expand_state->expanded_token->content[expand_state->current_pos] != '\"'
		)
	{
		// update_quote_state(&expand_state->quote_state, expand_state->expanded_token->content[expand_state->current_pos]);
		// if 
		expand_state->current_pos++;
	}
	// printf("name: %s\n", ft_substr(expand_state->expanded_token->content, start, expand_state->current_pos - start));
	env = get_env(ft_substr(expand_state->expanded_token->content, start, expand_state->current_pos - start), envs);
	return (env);
}

char	*expand_env_vals(t_expand_state *expand_state, t_envs *envs)
{
	char	*env;
	char	*expanded;

	expand_state->current_pos = 0;
	expand_state->trim_start = 0;
	expanded = ft_strdup("");
	while (expand_state->expanded_token->content[expand_state->current_pos])
	{
		// TODO: Add process when `echo "$"USER` and echo $
		if (expand_state->expanded_token->content[expand_state->current_pos] == '$')
		{
			if (expand_state->trim_start != expand_state->current_pos)
				expanded = ft_strjoin(expanded, ft_substr(expand_state->expanded_token->content, expand_state->trim_start, expand_state->current_pos - expand_state->trim_start));
			// TODO: Correspond $?
			expand_state->current_pos++;
			env = get_env_from_extracted_word(expand_state, env, expand_state->current_pos, envs);
			if (env != NULL)
				expanded = ft_strjoin(expanded, env);
			expand_state->trim_start = expand_state->current_pos;
			continue ;
		}
		expand_state->current_pos++;
	}
	if (expand_state->trim_start < expand_state->current_pos)
		expanded = ft_strjoin(expanded, ft_substr(expand_state->expanded_token->content, expand_state->trim_start, expand_state->current_pos - expand_state->trim_start));
	// printf("expanded: %s start: %ld current: %ld\n", expanded, expand_state->trim_start, expand_state->current_pos);
	return (expanded);
}

void	init_expand_state(t_expand_state *expand_state)
{
	expand_state->trim_start = 0;
	expand_state->current_pos = 0;
	expand_state->quote_state = NORMAL;
	expand_state->token_kind = TK_WORD;
}

int	expand(t_list *token_list, t_list **expanded_list, t_envs *envs)
{
	t_expand_state	expand_state;

	init_expand_state(&expand_state);
	while (token_list != NULL)
	{
		expand_state.expanded_token = (t_token *)malloc(sizeof(t_token));
		expand_state.expanded_token->attr = ((t_token *)token_list->content)->attr;
		expand_state.expanded_token->content = ((t_token *)token_list->content)->content;
		if ((expand_state.expanded_token->attr == TK_WORD || expand_state.expanded_token->attr == TK_DOUBLE_QUOTED) && ft_strchr(expand_state.expanded_token->content, '$') != NULL)
			expand_state.expanded_token->content = expand_env_vals(&expand_state, envs);
		if (expand_state.expanded_token->attr == TK_SINGLE_QUOTED)
			expand_state.expanded_token->content = ft_strtrim(expand_state.expanded_token->content, "\'");
		else if (expand_state.expanded_token->attr == TK_DOUBLE_QUOTED)
			expand_state.expanded_token->content = ft_strtrim(expand_state.expanded_token->content, "\"");
		if (ft_lstadd_node(expanded_list, expand_state.expanded_token) == FAIL)
			return (FAIL);
		token_list = token_list->next;
	}
	return (SUCCESS);
}

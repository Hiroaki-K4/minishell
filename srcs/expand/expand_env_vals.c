#include "minishell.h"

char	*get_name_after_dollar(t_expand_state *e_state, size_t start)
{
	while (e_state->origin_token->content[e_state->current_pos]
		&& e_state->origin_token->content[e_state->current_pos] != ' '
		&& e_state->origin_token->content[e_state->current_pos] != '$'
		&& e_state->origin_token->content[e_state->current_pos] != '\''
		&& e_state->origin_token->content[e_state->current_pos] != '\"'
	)
	{
		e_state->current_pos++;
		if (e_state->origin_token->content[e_state->current_pos - 1] == '?')
		{
			// TODO: Correspond $?
			printf("$? appeared\n");
			continue ;
		}
	}
	if (e_state->current_pos == start)
		return (ft_strdup("$"));
	return (ft_substr(e_state->origin_token->content,
			start, e_state->current_pos - start));
}

void	check_diff_between_start_and_curernt_pos(
	t_expand_state *e_state,
	char **expanded
)
{
	if (e_state->start != e_state->current_pos)
		*expanded = ft_strjoin(*expanded,
				ft_substr(e_state->origin_token->content,
					e_state->start, e_state->current_pos - e_state->start));
}

char	*expand_env_vals(t_expand_state *e_state, t_envs *envs)
{
	char	*name;
	char	*value;
	char	*tmp;
	t_token	*new_token;
	t_list	*tmp_list;
	t_list	*last_list;

	// TODO: Remove tmp
	init_expand_state(e_state);
	tmp = ft_strdup("");
	while (e_state->origin_token->content[e_state->current_pos])
	{
		update_quote_state(&(e_state->quote_state),
			e_state->origin_token->content[e_state->current_pos]);
		if (e_state->origin_token->content[e_state->current_pos] == '$'
			&& e_state->quote_state != IN_QUOTE)
		{
			check_diff_between_start_and_curernt_pos(e_state, &tmp);
			e_state->current_pos++;
			name = get_name_after_dollar(e_state, e_state->current_pos);
			if (ft_strncmp(name, "$", ft_strlen(name) + 1) == 0)
			{
				last_list = ft_lstlast(e_state->token_list);
				// TODO: Make function
				if (last_list != NULL)
					last_list->content = ft_strjoin(last_list->content, name);
				else
				{
					new_token = make_token(name, 0, ft_strlen(name), TK_WORD);
					if (ft_lstadd_node(&(e_state->token_list), new_token) == FAIL)
						return (NULL);
				}
				tmp = ft_strjoin(tmp, name);
			}
			else
			{
				value = get_env(name, envs);
				// printf("value: %s\n", value);
				if (value != NULL)
				{
					if (e_state->quote_state == NORMAL)
					{
						tmp_list = NULL;
						tokenize(value, &tmp_list);
						ft_lstadd_back(&(e_state->token_list), tmp_list);
					}
					else
					{	
						new_token = make_token(value, 0, ft_strlen(value), TK_WORD);
						if (ft_lstadd_node(&(e_state->token_list), new_token) == FAIL)
							return (NULL);
					}
					// printf("~~~after_tokenize~~~\n");
					// ft_lstiter(e_state->token_list, output_result);
					tmp = ft_strjoin(tmp, value);
				}
			}
			e_state->start = e_state->current_pos;
			continue ;
		}
		e_state->current_pos++;
	}
	printf("~~~after_tokenize~~~\n");
	ft_lstiter(e_state->token_list, output_result);
	check_diff_between_start_and_curernt_pos(e_state, &tmp);
	return (tmp);
}

// char	*expand_env_vals(t_expand_state *e_state, t_envs *envs)
// {
// 	char	*name;
// 	char	*value;
// 	char	*expanded;

// 	init_expand_state(e_state);
// 	expanded = ft_strdup("");
// 	while (e_state->origin_token->content[e_state->current_pos])
// 	{
// 		update_quote_state(&(e_state->quote_state),
// 			e_state->origin_token->content[e_state->current_pos]);
// 		if (e_state->origin_token->content[e_state->current_pos] == '$'
// 			&& e_state->quote_state != IN_QUOTE)
// 		{
// 			check_diff_between_start_and_curernt_pos(e_state, &expanded);
// 			e_state->current_pos++;
// 			name = get_name_after_dollar(e_state, e_state->current_pos);
// 			if (ft_strncmp(name, "$", ft_strlen(name) + 1) == 0)
// 				expanded = ft_strjoin(expanded, name);
// 			else
// 			{
// 				value = get_env(name, envs);
// 				printf("value: %s\n", value);
// 				if (value != NULL)
// 				{
// 					tokenize(value, &(e_state->token_list));
// 					printf("~~~after_tokenize~~~\n");
// 					ft_lstiter(e_state->token_list, output_result);
// 					expanded = ft_strjoin(expanded, value);
// 				}
// 			}
// 			e_state->start = e_state->current_pos;
// 			continue ;
// 		}
// 		e_state->current_pos++;
// 	}
// 	check_diff_between_start_and_curernt_pos(e_state, &expanded);
// 	return (expanded);
// }

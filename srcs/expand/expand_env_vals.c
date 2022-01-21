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

int	ft_lstadd_word(t_list **lst, char *new_word)
{
	t_list	*last_lst;
	t_token	*new_token;
	t_token	*last_token;

	if (!*lst)
	{
		new_token = make_token(new_word, 0, ft_strlen(new_word), TK_WORD);
		if (ft_lstadd_node(lst, new_token) == FAIL)
			return (FAIL);
		return (SUCCESS);
	}
	else
	{
		last_lst = ft_lstlast(*lst);
		last_token = (t_token *)last_lst->content;
		last_token->content = ft_strjoin(last_token->content, new_word);
		if (last_token->content == NULL)
			return (FAIL);
	}
	return (SUCCESS);
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
	ft_lstadd_word(&(e_state->token_list), ft_substr(e_state->origin_token->content,
					e_state->start, e_state->current_pos - e_state->start));
}

void	ft_lstadd_last(t_list **lst, t_list *new)
{
	t_list	*last_lst;
	t_token	*new_token;
	t_token	*last_token;
	char	*new_word;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last_lst = ft_lstlast(*lst);
	new_token = (t_token *)new->content;
	new_word = new_token->content;
	last_token = (t_token *)last_lst->content;
	last_token->content = ft_strjoin(last_token->content, new_word);
	if (new->next != NULL)
	{
		new->next->prev = last_lst;
		last_lst->next = new->next;
	}
	else
		last_lst->next = NULL;
}

char	*expand_env_vals(t_expand_state *e_state, t_envs *envs)
{
	char	*name;
	char	*value;
	char	*tmp;
	t_list	*tmp_list;

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
				ft_lstadd_word(&(e_state->token_list), name);
				tmp = ft_strjoin(tmp, name);
			}
			else
			{
				value = get_env(name, envs);
				if (value != NULL)
				{
					if (e_state->quote_state == NORMAL)
					{
						tmp_list = NULL;
						tokenize(value, &tmp_list);
						ft_lstadd_last(&(e_state->token_list), tmp_list);
					}
					else
						ft_lstadd_word(&(e_state->token_list), value);
					tmp = ft_strjoin(tmp, value);
				}
			}
			e_state->start = e_state->current_pos;
			continue ;
		}
		e_state->current_pos++;
	}
	check_diff_between_start_and_curernt_pos(e_state, &tmp);
	printf("~~~after_tokenize~~~\n");
	ft_lstiter(e_state->token_list, output_result);
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

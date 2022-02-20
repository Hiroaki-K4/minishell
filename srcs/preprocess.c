#include "minishell.h"

void	output_result(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	printf("content: %s attr: %d\n", (char *)token->content, token->attr);
}

void	print_debug(t_list *token_list, t_list *expanded_list)
{
	printf("~~~~~After tokenize~~~~~\n");
	ft_lstiter(token_list, output_result);
	printf("~~~~~After expand~~~~~\n");
	ft_lstiter(expanded_list, output_result);
}

t_list	*get_tokenized_list(char *line)
{
	char		*trimed;
	t_list		*token_list;

	token_list = NULL;
	trimed = ft_strtrim(line, " \t");
	if (!trimed)
		return (NULL);
	if (tokenize(trimed, &token_list) == FAIL)
	{
		free(trimed);
		ft_lstclear_all(&token_list, free);
		return (NULL);
	}
	free(trimed);
	return (token_list);
}

t_node	*preprocess(char *line, t_global_state *state, int is_debug_mode)
{
	t_list		*token_list;
	t_list		*expanded_list;

	token_list = get_tokenized_list(line);
	if (token_list == NULL)
		return (NULL);
	if (check_syntax(token_list) == FAIL)
	{
		ft_lstclear_all(&token_list, free);
		return (NULL);
	}
	expanded_list = NULL;
	if (expand(token_list, &expanded_list, state->envs,
			state->last_command_exit_status) == FAIL)
	{
		ft_lstclear_all(&token_list, free);
		return (NULL);
	}
	if (is_debug_mode)
		print_debug(token_list, expanded_list);
	ft_lstclear_all(&token_list, free);
	return (parse(&expanded_list));
}

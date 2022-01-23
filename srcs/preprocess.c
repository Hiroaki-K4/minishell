#include "minishell.h"

void	output_result(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	printf("content: %s attr: %d\n", (char *)token->content, token->attr);
}

t_node	*preprocess(char *line, t_global_state *state)
{
	char		*trimed;
	t_list		*token_list;
	t_list		*expanded_list;

	token_list = NULL;
	trimed = ft_strtrim(line, " \t");
	if (tokenize(trimed, &token_list) == FAIL)
		return (NULL);
	free(trimed);
	if (token_list == NULL)
		return (NULL);
	if (check_syntax(token_list) == FAIL)
		return (NULL);
	printf("~~~~~After tokenize~~~~~\n");
	ft_lstiter(token_list, output_result);
	expanded_list = NULL;
	expand(token_list, &expanded_list, state->envs,
		state->last_command_exit_status);
	ft_lstclear_all(&token_list, free);
	printf("~~~~~After expand~~~~~\n");
	ft_lstiter(expanded_list, output_result);
	return (parse(&expanded_list));
}

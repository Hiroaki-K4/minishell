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
	tokenize(trimed, &token_list);
	if (token_list == NULL)
		return (NULL);
	if (check_syntax(token_list) == FAIL)
		return (NULL);
	ft_lstiter(token_list, output_result);
	expanded_list = NULL;
	expand(token_list, &expanded_list, state->envs);
	return (parse(&expanded_list));
}

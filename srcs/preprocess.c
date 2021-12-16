#include "minishell.h"

void	output_result(void *content)
{
	t_token	*token;

	token = (t_token *)content;
	printf("content: %s attr: %d\n", (char *)token->content, token->attr);
}

int	preprocess(char *line)
{
	char		*trimed;
	t_list		*token_list;

	token_list = NULL;
	trimed = ft_strtrim(line, " \t");
	tokenize(trimed, &token_list);
	if (check_syntax(token_list) == FAIL)
		return (FAIL);
	ft_lstiter(token_list, output_result);
	parse(&token_list);
	return (SUCCESS);
}

#include "minishell.h"

char	*expand_env_vals(char *content)
{
	int	i;
	int	start;
	char	*expanded;

	i = 0;
	start = 0;
	while (content[i])
	{
		if (content[i] == '$' && start != i)
		{
			expanded = ft_strjoin(expanded, ft_substr(content, start, i - start));
			printf("expanded: %s\n", expanded);
			start = i;
		}
		i++;
	}
	return (expanded);
}

int	expand(t_list *token_list, t_list **expanded_list)
{
	t_token	*expanded_token;

	(void)expanded_list;
	expanded_token = (t_token *)malloc(sizeof(t_token));
	while (token_list->next != NULL)
	{
		if (((t_token *)(token_list->content))->attr == TK_SINGLE_QUOTED)
			expanded_token->content = ft_strtrim(((t_token *)token_list->content)->content, "\'");
		else if (((t_token *)(token_list->content))->attr == TK_DOUBLE_QUOTED)
			expanded_token->content = ft_strtrim(((t_token *)token_list->content)->content, "\"");
		else
			expanded_token->content = ((t_token *)token_list->content)->content;
		expanded_token->content = expand_env_vals(expanded_token->content);
		expanded_token->attr = ((t_token *)token_list->content)->attr;
		if (ft_lstadd_node(expanded_list, expanded_token) == FAIL)
			return (FAIL);
		token_list = token_list->next;
	}
	if (token_list != NULL)
		printf("final\n");
	return (SUCCESS);
}

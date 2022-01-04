#include "minishell.h"

int	get_next_dollar_pos(char *content, int pos)
{
	pos++;
	while (content[pos] && content[pos] != '$')
		pos++;
	return (pos);
}

int	get_env_first_pos(char *content, char **env, int start, int next_dollar)
{
	int	env_pos;

	env_pos = start;
	while (env_pos < next_dollar && content[env_pos] != ' ')
		env_pos++;
	*env = getenv(ft_substr(content, start, env_pos - start));
	return (env_pos);
}

char	*expand_env_vals(char *content)
{
	int		i;
	int		start;
	int		env_pos;
	char	*env;
	char	*expanded;

	i = 0;
	start = 0;
	expanded = ft_strdup("");
	while (content[i])
	{
		if (content[i] == '$')
		{
			if (start != i)
				expanded = ft_strjoin(expanded, ft_substr(content, start, i - start));
			// TODO: Correspond $?
			env_pos = get_env_first_pos(content, &env, i + 1, get_next_dollar_pos(content, i));
			if (env != NULL)
				expanded = ft_strjoin(expanded, env);
			i = env_pos;
			start = env_pos;
			continue ;
		}
		i++;
	}
	return (expanded);
}

int	expand(t_list *token_list, t_list **expanded_list)
{
	t_token	*expanded_token;

	while (token_list != NULL)
	{
		expanded_token = (t_token *)malloc(sizeof(t_token));
		expanded_token->attr = ((t_token *)token_list->content)->attr;
		if (expanded_token->attr == TK_SINGLE_QUOTED)
			expanded_token->content = ft_strtrim(((t_token *)token_list->content)->content, "\'");
		else if (expanded_token->attr == TK_DOUBLE_QUOTED)
			expanded_token->content = ft_strtrim(((t_token *)token_list->content)->content, "\"");
		else
			expanded_token->content = ((t_token *)token_list->content)->content;
		if ((expanded_token->attr == TK_WORD || expanded_token->attr == TK_DOUBLE_QUOTED) && ft_strchr(expanded_token->content, '$') != NULL)
			expanded_token->content = expand_env_vals(expanded_token->content);
		if (ft_lstadd_node(expanded_list, expanded_token) == FAIL)
			return (FAIL);
		token_list = token_list->next;
	}
	return (SUCCESS);
}

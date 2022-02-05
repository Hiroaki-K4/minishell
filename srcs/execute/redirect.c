#include "minishell.h"

int	is_redirect_token(t_token *token)
{
	return (token->attr == TK_IO_NUMBER
		|| token->attr == TK_REDIRECT_IN
		|| token->attr == TK_REDIRECT_OUT
		|| token->attr == TK_REDIRECT_DGREAT
		|| token->attr == TK_REDIRECT_DLESS);
}

void	init_redirect(t_redirect *redirect)
{
	redirect->redirect_fd = -1;
	redirect->file_fd = -1;
	redirect->here_delimiter = NULL;
	redirect->here_document = NULL;
}

static void	handle_pattern1(
	t_redirect *redirect,
	char *content,
	t_token_kind attr
)
{
	if (attr == TK_REDIRECT_OUT)
	{
		redirect->file_fd = open(content, O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (redirect->redirect_fd == -1)
			redirect->redirect_fd = 1;
	}
	else if (attr == TK_REDIRECT_DGREAT)
	{
		redirect->file_fd = open(content, O_RDWR | O_CREAT | O_APPEND, 0666);
		if (redirect->redirect_fd == -1)
			redirect->redirect_fd = 1;
	}
	else if (attr == TK_REDIRECT_IN)
	{
		redirect->file_fd = open(content, O_RDONLY);
		if (redirect->redirect_fd == -1)
			redirect->redirect_fd = 0;
	}
	else
		exit_with_error("invaild redirect type");
	if (redirect->file_fd < 0)
		exit_with_error(content);
}

static void	handle_pattern2(
	t_redirect *redirect,
	char *content,
	t_envs *envs
)
{
	char	*input;
	char	*tmp;
	char	*tmp_fp;

	redirect->here_delimiter = content;
	input = readline("> ");
	redirect->here_document = input;
	while (ft_strncmp(input, redirect->here_delimiter, ft_strlen(redirect->here_delimiter) + 1))
	{
		input = readline("> ");
		if (!ft_strncmp(input, redirect->here_delimiter, ft_strlen(redirect->here_delimiter) + 1))
			break ;
		tmp = ft_strjoin(redirect->here_document, "\n");
		free(redirect->here_document);
		redirect->here_document = ft_strjoin(tmp, input);
	}
	tmp = ft_strjoin(redirect->here_document, "\n");
	free(redirect->here_document);
	redirect->here_document = tmp;
	tmp = get_env("PWD", envs);
	tmp_fp = ft_strjoin(tmp, "/minishell_tmp");
	free(tmp);
	redirect->file_fd = open(tmp_fp, O_RDWR | O_CREAT, 0666);
	write(redirect->file_fd, redirect->here_document, ft_strlen(redirect->here_document));
	close(redirect->file_fd);
	redirect->file_fd = open(tmp_fp, O_RDWR | O_CREAT, 0666);
	unlink(tmp_fp);
	free(tmp_fp);
	if (redirect->redirect_fd == -1)
		redirect->redirect_fd = 0;
}

int	set_redirect(t_list **tokens, t_redirect *redirect, t_envs *envs)
{
	char			*content;
	t_token_kind	attr;

	content = NULL;
	attr = ((t_token *)((*tokens)->content))->attr;
	if (attr == TK_REDIRECT_OUT
		|| attr == TK_REDIRECT_DGREAT
		|| attr == TK_REDIRECT_IN)
	{
		*tokens = (*tokens)->next;
		if (*tokens == NULL)
			return (FAIL);
		content = ((t_token *)((*tokens)->content))->content;
		handle_pattern1(redirect, content, attr);
	}
	else if (attr == TK_REDIRECT_DLESS)
	{
		*tokens = (*tokens)->next;
		if (*tokens == NULL)
			return (FAIL);
		content = ((t_token *)((*tokens)->content))->content;
		handle_pattern2(redirect, content, envs);
	}
	*tokens = (*tokens)->next;
	return (SUCCESS);
}

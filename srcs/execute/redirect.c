#include "minishell.h"

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

static void	read_heredocument(t_redirect *redirect, char *content)
{
	size_t	len;
	char	*input;
	char	*tmp;

	len = ft_strlen(content);
	redirect->here_delimiter = ft_strdup(content);
	input = readline("> ");
	redirect->here_document = input;
	while (ft_strncmp(input, redirect->here_delimiter, len + 1))
	{
		input = readline("> ");
		if (!ft_strncmp(input, redirect->here_delimiter, len + 1))
			break ;
		tmp = ft_strjoin(redirect->here_document, "\n");
		free(redirect->here_document);
		redirect->here_document = ft_strjoin(tmp, input);
		free(tmp);
	}
	tmp = ft_strjoin(redirect->here_document, "\n");
	free(redirect->here_document);
	redirect->here_document = tmp;
}

static void	handle_pattern2(
	t_redirect *redirect,
	char *content,
	t_envs *envs
)
{
	size_t	len;
	char	*tmp;
	char	*tmp_fp;

	read_heredocument(redirect, content);
	tmp = get_env("PWD", envs);
	tmp_fp = ft_strjoin(tmp, "/minishell_tmp");
	free(tmp);
	redirect->file_fd = open(tmp_fp, O_RDWR | O_CREAT, 0666);
	len = ft_strlen(redirect->here_document);
	write(redirect->file_fd, redirect->here_document, len);
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

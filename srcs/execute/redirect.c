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

void	set_redirect(t_list **tokens, t_redirect *redirect, t_envs *envs)
{
	char	*input;
	char	*tmp_fp;

	if (*tokens == NULL)
		return ;
	if (((t_token *)((*tokens)->content))->attr == TK_IO_NUMBER)
	{
		redirect->redirect_fd = ft_atoi(((t_token *)((*tokens)->content))->content);
		*tokens = (*tokens)->next;
	}
	if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_OUT)
	{
		*tokens = (*tokens)->next;
		redirect->file_fd = open(((t_token *)((*tokens)->content))->content, O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (redirect->file_fd < 0)
			exit_with_error(((t_token *)((*tokens)->content))->content);
		if (redirect->redirect_fd == -1)
			redirect->redirect_fd = 1;
	}
	else if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_IN)
	{
		*tokens = (*tokens)->next;
		redirect->file_fd = open(((t_token *)((*tokens)->content))->content, O_RDONLY);
		if (redirect->file_fd < 0)
			exit_with_error(((t_token *)((*tokens)->content))->content);
		if (redirect->redirect_fd == -1)
			redirect->redirect_fd = 0;
	}
	else if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_DGREAT)
	{
		*tokens = (*tokens)->next;
		redirect->file_fd = open(((t_token *)((*tokens)->content))->content, O_RDWR | O_CREAT | O_APPEND, 0666);
		if (redirect->file_fd < 0)
			exit_with_error(((t_token *)((*tokens)->content))->content);
		if (redirect->redirect_fd == -1)
			redirect->redirect_fd = 1;
	}
	else if (((t_token *)((*tokens)->content))->attr == TK_REDIRECT_DLESS)
	{
		*tokens = (*tokens)->next;
		redirect->here_delimiter = ((t_token *)((*tokens)->content))->content;
		input = readline("> ");
		redirect->here_document = input;
		while (ft_strncmp(input, redirect->here_delimiter, ft_strlen(redirect->here_delimiter) + 1))
		{
			input = readline("> ");
			if (!ft_strncmp(input, redirect->here_delimiter, ft_strlen(redirect->here_delimiter) + 1))
				break ;
			redirect->here_document = ft_strjoin(redirect->here_document, ft_strjoin(ft_strdup("\n"), input));
		}
		redirect->here_document = ft_strjoin(redirect->here_document, ft_strdup("\n"));
		tmp_fp = ft_strjoin(get_env("PWD", envs), "/minishell_tmp");
		redirect->file_fd = open(tmp_fp, O_RDWR | O_CREAT, 0666);
		write(redirect->file_fd, redirect->here_document, ft_strlen(redirect->here_document));
		close(redirect->file_fd);
		redirect->file_fd = open(tmp_fp, O_RDWR | O_CREAT, 0666);
		unlink(tmp_fp);
		if (redirect->redirect_fd == -1)
			redirect->redirect_fd = 0;
	}
	*tokens = (*tokens)->next;
}

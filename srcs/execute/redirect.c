/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:48:23 by hkubo             #+#    #+#             */
/*   Updated: 2022/03/27 20:18:57 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_pattern1(
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
	if (redirect->file_fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(content);
		return (FAIL);
	}
	return (SUCCESS);
}

static int	redirect_to_tmpfile(t_redirect *redirect, char *fp)
{
	size_t	heredoc_len;
	char	*tmp_fp;

	tmp_fp = "/tmp/minishell_tmp";
	redirect->file_fd = open(tmp_fp, O_RDWR | O_CREAT, 0666);
	if (redirect->file_fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(fp);
		return (FAIL);
	}
	heredoc_len = ft_strlen(redirect->here_document);
	write(redirect->file_fd, redirect->here_document, heredoc_len);
	close(redirect->file_fd);
	redirect->file_fd = open(tmp_fp, O_RDWR | O_CREAT, 0666);
	unlink(tmp_fp);
	return (SUCCESS);
}

static int	handle_pattern2(
	t_redirect *redirect,
	char *file_fp,
	t_envs *envs,
	int exit_status
)
{
	int	ret;

	if (read_heredocument(redirect, file_fp, envs, exit_status) == FAIL)
		return (FAIL);
	ret = redirect_to_tmpfile(redirect, file_fp);
	if (redirect->redirect_fd == -1)
		redirect->redirect_fd = 0;
	return (ret);
}

int	set_redirect(t_list **tokens, t_redirect *redirect, t_envs *envs, int exit_status)
{
	int				ret;
	char			*content;
	t_token_kind	attr;

	ret = SUCCESS;
	attr = ((t_token *)((*tokens)->content))->attr;
	if (attr == TK_REDIRECT_OUT || attr == TK_REDIRECT_DGREAT
		|| attr == TK_REDIRECT_IN)
	{
		*tokens = (*tokens)->next;
		if (*tokens == NULL)
			return (FAIL);
		content = ((t_token *)((*tokens)->content))->content;
		ret = handle_pattern1(redirect, content, attr);
	}
	else if (attr == TK_REDIRECT_DLESS)
	{
		*tokens = (*tokens)->next;
		if (*tokens == NULL)
			return (FAIL);
		content = ((t_token *)((*tokens)->content))->content;
		redirect->q_removed = ((t_token *)((*tokens)->content))->q_removed;
		ret = handle_pattern2(redirect, content, envs, exit_status);
	}
	*tokens = (*tokens)->next;
	return (ret);
}

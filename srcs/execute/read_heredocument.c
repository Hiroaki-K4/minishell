/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredocument.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkubo <hkubo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 22:48:15 by hkubo             #+#    #+#             */
/*   Updated: 2022/02/21 22:48:16 by hkubo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	concat_heredocument(t_redirect *redirect, char **input)
{
	char	*tmp;

	if (redirect->here_document == NULL)
		redirect->here_document = *input;
	else
	{
		tmp = ft_strjoin(redirect->here_document, "\n");
		free(redirect->here_document);
		redirect->here_document = ft_strjoin(tmp, *input);
		free(tmp);
		free(*input);
		*input = NULL;
	}
}

static void	shape_heredocument(t_redirect *redirect)
{
	char	*tmp;

	if (redirect->here_document == NULL)
		redirect->here_document = ft_strdup("");
	else
	{
		tmp = ft_strjoin(redirect->here_document, "\n");
		free(redirect->here_document);
		redirect->here_document = tmp;
	}
}

int	read_heredocument(t_redirect *redirect, char *content)
{
	int		ret;
	size_t	len;
	char	*input;

	len = ft_strlen(content);
	redirect->here_delimiter = ft_strdup(content);
	input = NULL;
	while (TRUE)
	{
		ret = get_next_line(0, &input);
		if (ret < 0)
			return (FAIL);
		if (ret == 0 || !ft_strncmp(input, redirect->here_delimiter, len + 1))
			break ;
		concat_heredocument(redirect, &input);
	}
	free(input);
	shape_heredocument(redirect);
	return (SUCCESS);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychida <ychida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/14 15:32:45 by ychida            #+#    #+#             */
/*   Updated: 2022/02/06 20:46:13 by ychida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	oneline_len(char *s)
{
	int	len;

	len = 0;
	while (s[len] != '\n' && s[len] != '\0')
		len++;
	return (len);
}

static int	store_oneline(int fd, char **stored_line)
{
	int		ret;
	char	*buf;
	char	*tmp;

	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buf == NULL)
		return (-1);
	ret = read(fd, buf, BUFFER_SIZE);
	while (ret > 0)
	{
		buf[ret] = '\0';
		tmp = ft_strjoin(stored_line[fd], buf);
		if (tmp == NULL)
		{
			ret = -1;
			break ;
		}
		free(stored_line[fd]);
		stored_line[fd] = tmp;
		if (ft_strchr(stored_line[fd], '\n'))
			break ;
		ret = read(fd, buf, BUFFER_SIZE);
	}
	free(buf);
	return (ret);
}

static int	_extract_line(int fd, char **line, char **stored_line, int len_nl)
{
	char	*tmp;

	if ((len_nl + 1) != (int)ft_strlen(stored_line[fd]))
	{
		tmp = ft_strdup(stored_line[fd] + len_nl + 1);
		if (tmp == NULL)
		{
			free(line);
			return (-1);
		}
		free(stored_line[fd]);
		stored_line[fd] = tmp;
	}
	else
	{
		free(stored_line[fd]);
		stored_line[fd] = NULL;
	}
	return (0);
}

static int	extract_line(int fd, char **line, char **stored_line)
{
	int	ret;
	int	len_nl;
	int	is_just_nl;

	is_just_nl = 0;
	len_nl = oneline_len(stored_line[fd]);
	if (stored_line[fd][len_nl] == '\n')
		is_just_nl = 1;
	*line = ft_substr(stored_line[fd], 0, len_nl);
	if (*line == NULL)
		return (-1);
	if (is_just_nl)
	{
		ret = _extract_line(fd, line, stored_line, len_nl);
		if (ret < 0)
			return (ret);
	}
	return (is_just_nl);
}

int	get_next_line(int fd, char **line)
{
	int			ret;
	static char	*stored_line[OPEN_MAX + 1];

	if (fd < 0 || OPEN_MAX < fd || line == NULL || BUFFER_SIZE < 1)
		return (FAIL);
	write(1, REDIRECT_PROMPT, ft_strlen(REDIRECT_PROMPT) + 1);
	if (stored_line[fd] == NULL)
	{
		stored_line[fd] = ft_strdup("");
		if (stored_line[fd] == NULL)
			return (FAIL);
	}
	if (ft_strchr(stored_line[fd], '\n'))
		ret = 1;
	else
		ret = store_oneline(fd, stored_line);
	if (ret >= 0)
		ret = extract_line(fd, line, stored_line);
	if (ret <= 0)
	{
		free(stored_line[fd]);
		stored_line[fd] = NULL;
	}
	return (ret);
}

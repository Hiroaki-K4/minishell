/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychida <ychida@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 15:57:06 by ychida            #+#    #+#             */
/*   Updated: 2022/03/27 21:26:48 by ychida           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	new_ptr = malloc(size);
	if (new_ptr == NULL)
		return (NULL);
	if (ptr)
	{
		ft_memcpy(new_ptr, ptr, size);
		free(ptr);
	}
	return (new_ptr);
}

void	**ft_realloc_array(void **ptr, size_t element_num)
{
	size_t	i;
	void	**new_ptr;

	new_ptr = (void **)malloc(sizeof(void *) * (element_num + 1));
	if (new_ptr == NULL)
		return (NULL);
	if (!ptr)
		return (new_ptr);
	ft_bzero(new_ptr, sizeof(void *) * (element_num + 1));
	i = 0;
	while (ptr[i])
	{
		new_ptr[i] = ptr[i];
		i++;
	}
	while (i < element_num)
	{
		new_ptr[i] = NULL;
		i++;
	}
	return (new_ptr);
}

void	realloc_redirects(t_global_state *state)
{
	state->max_redirect_num += 100;
	state->redirects = (t_redirect **)ft_realloc_array(
			(void **)state->redirects, state->max_redirect_num);
}

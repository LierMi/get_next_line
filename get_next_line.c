/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liemi <liemi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:43:53 by liemi             #+#    #+#             */
/*   Updated: 2025/09/17 15:45:29 by liemi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*update_stash(char *stash)
{
	char	*new_stash;
	size_t	i;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	new_stash = gnl_subdup(stash, i + 1);
	free(stash);
	return (new_stash);
}

static int	gnl_merge(char **st, char *buf, ssize_t n)
{
	char	*new;
	size_t	len;

	if (n <= 0)
		return (0);
	buf[n] = '\0';
	len = ft_strlen(*st);
	new = (char *)malloc(len + n + 1);
	if (!new)
		return (-1);
	if (*st)
	{
		ft_memcpy(new, *st, len);
		free(*st);
	}
	ft_memcpy(new + len, buf, n + 1);
	*st = new;
	return (0);
}

static int	gnl_read(int fd, char **st)
{
	char	*buf;
	ssize_t	r;

	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (-1);
	r = 1;
	while (!ft_strchr(*st, '\n') && r > 0)
	{
		r = read(fd, buf, BUFFER_SIZE);
		if (r < 0 || gnl_merge(st, buf, r) < 0)
		{
			free(buf);
			return (-1);
		}
	}
	free(buf);
	return (0);
}

char	*get_next_line(int fd)
{
	static char	*st;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (gnl_read(fd, &st) < 0)
	{
		free(st);
		st = NULL;
		return (NULL);
	}
	line = extract_line(st);
	st = update_stash(st);
	return (line);
}

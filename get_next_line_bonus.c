/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liemi <liemi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:31:12 by liemi             #+#    #+#             */
/*   Updated: 2025/08/04 18:57:28 by liemi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static t_fdnode	*get_fd_node(t_fdnode **head, int fd)
{
	t_fdnode	*current;

	current = *head;
	while (current)
	{
		if (current->fd == fd)
			return (current);
		current = current->next;
	}
	current = malloc(sizeof(t_fdnode));
	if (!current)
		return (NULL);
	current->fd = fd;
	current->stash = NULL;
	current->next = *head;
	*head = current;
	return (current);
}

static void	remove_fd_node(t_fdnode **head, int fd)
{
	t_fdnode	*prev;
	t_fdnode	*curr;

	prev = NULL;
	curr = *head;
	while (curr)
	{
		if (curr->fd == fd)
		{
			if (prev)
				prev->next = curr->next;
			else
				*head = curr->next;
			free(curr->stash);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

char	*get_next_line(int fd)
{
	static t_fdnode	*head;
	t_fdnode		*node;
	char			*buffer;
	ssize_t			bytes_read;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	node = get_fd_node(&head, fd);
	if (!node)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(node->stash, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			remove_fd_node(&head, fd);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		node->stash = ft_strjoin(node->stash, buffer);
	}
	free(buffer);
	line = extract_line(node->stash);
	node->stash = update_stash(node->stash);
	if(!line)
		remove_fd_node(&head, fd);
	return (line);
}
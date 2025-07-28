/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liemi <liemi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:43:53 by liemi             #+#    #+#             */
/*   Updated: 2025/07/16 22:51:46 by liemi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	char		*line;
	ssize_t		bytes_read;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(stash, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		temp = stash;
		stash = ft_strjoin(stash, buffer);
		free(temp);
	}
	free(buffer);
	line = extract_line(stash);
	stash = update_stash(stash);
	return (line);
}

// stash 是保存还没读完的残留数据，每次调用保持不变
// buffer 是每次 read 读入的新数据
// temp 是临时指针，用于释放旧的 stash

char	*extract_line(char *stash)
{
	char 	*line;
	size_t	i;

	if (!stash || stash[0] == '\0')
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	line = (char *)malloc(i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
	{
		line[i] = '\n';
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*update_stash(char *stash)
{
	char	*new_stash;
	size_t	i;
	size_t	j;

	if (!stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\0')
	{
		free(stash);
		return (NULL);
	}
	i++;
	new_stash = (char *)malloc(ft_strlen(stash) - i + 1);
	if (!new_stash)
		return (NULL);
	j = 0;
	while (stash[i])
	{
		new_stash[j] = stash[i];
		i++;
		j++;
	}
	new_stash[j] = '\0';
	free(stash);
	return (new_stash);
}
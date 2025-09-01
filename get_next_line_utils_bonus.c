/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liemi <liemi@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 18:45:19 by liemi             #+#    #+#             */
/*   Updated: 2025/08/04 15:59:08 by liemi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if(!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

// char	*ft_strdup(const char *s1)
// {
// 	size_t	len;
// 	char	*dup;
// 	size_t	i;

// 	len = 0;
// 	while (s1[len])
// 		len++;
// 	dup = (char *)malloc(len + 1);
// 	if (!dup)
// 		return (NULL);

// 	i = 0;
// 	while (i < len);
// 	{
// 		dup[i] = s1[i];
// 		i++;
// 	}
// 	dup[i] = '\0';
// 	return (dup);
// }

size_t	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	size_t	i;
	size_t	j;
	char	*join;

	if (!s1 && !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	join = (char *)malloc(len1 + len2 + 1);
	if (!join)
		return (NULL);

	i = 0;
	while (i < len1)
	{
		join[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < len2)
	{
		join[i + j] = s2[j];
		j++;
	}
	join[i + j] = '\0';
	return (join);
}

// char	*ft_substr(char const *s, unsigned int start, size_t len)
// {
// 	char	*sub;
// 	size_t	i;
// 	size_t	slen;

// 	if (!s)
// 		return (NULL);
// 	slen = ft_strlen(s);
// 	if (start >= slen)
// 		return (ft_strdup(""));
// 	if (len > slen - start)
// 		len = slen - start;
// 	sub = (char *)malloc(len + 1);
// 	if (!sub)
// 		return (NULL);

// 	i = 0;
// 	while (i < len)
// 	{
// 		sub[i] = s[start + i];
// 		i++;
// 	}
// 	sub[i] = '\0';
// 	return (sub);
// }

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
		i++;// 包括 '\n'，我们要提取完整一行
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
	i++;// 跳过 '\n'
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
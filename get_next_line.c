/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paubello <paubello@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 08:20:52 by paubello          #+#    #+#             */
/*   Updated: 2024/11/19 17:26:42 by paubello         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * Processes a line from the stash up to a newline character.
 *
 * @param stash The stash containing the data to process.
 * @param newline_pos The position of the newline character in the stash.
 * @return The processed line, or NULL if memory allocation fails.
 */
static char	*ft_process_line(char **stash, char *newline_pos)
{
	char	*line;
	char	*temp;
	size_t	len;

	len = newline_pos - *stash + 1;
	line = (char *)ft_calloc(len + 1, sizeof(char));
	if (!line)
		return (NULL);
	temp = *stash;
	while (len--)
		*line++ = *temp++;
	*line = '\0';
	line -= (newline_pos - *stash + 1);
	temp = ft_strjoin(temp, "");
	free(*stash);
	*stash = temp;
	return (line);
}

/**
 * Extracts the next line from the stash.
 *
 * @param stash The stash to extract the line from.
 * @return The extracted line, or NULL if the stash is empty or NULL.
 */
static char	*ft_extract_line(char **stash)
{
	char	*line;
	char	*newline_pos;

	if (!*stash || !**stash)
		return (NULL);
	newline_pos = ft_strchr(*stash, '\n');
	if (newline_pos)
		return (ft_process_line(stash, newline_pos));
	line = ft_strjoin(*stash, "");
	free(*stash);
	*stash = NULL;
	return (line);
}

/**
 * Reads from a file descriptor and appends the content to the stash.
 *
 * @param fd The file descriptor to read from.
 * @param stash A pointer to the stash, which stores accumulated data.
 * @return The updated stash with the newly read data appended, or NULL on
 *         error.
 */
static char	*ft_read_file(int fd, char **stash)
{
	char	*buffer;
	char	*temp;
	ssize_t	bytes_read;

	buffer = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(*stash, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(*stash, buffer);
		free(*stash);
		*stash = temp;
	}
	free(buffer);
	return (*stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash)
		stash = ft_calloc(1, sizeof(char));
	if (!stash)
		return (NULL);
	if (!ft_read_file(fd, &stash))
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	line = ft_extract_line(&stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
	}
	return (line);
}

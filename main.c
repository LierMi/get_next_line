#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int main(void)
{
	int fd = open("file.txt", O_RDONLY);
	char	*line;

	while ((line = get_next_line(fd) != NULL))
	{
		print("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}

int	main(void)
{
	int		fd1, fd2;
	char	*line;

	fd1 = open("file1.txt", O_RDONLY);
	fd2 = open("file2.txt", O_RDONLY);
	if (fd1 < 0 || fd2 < 0)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	printf("----Start reading----\n");

	while (1)
	{
		line = get_next_line(fd1);
		if (!line)
			break;
		printf("File1: %s", line);
		free(line);

		line = get_next_line(fd2);
		if (!line)
			break;
		printf("File2: %s", line);
		free(line);
	}

	close(fd1);
	close(fd2);

	printf("----Done----\n");
	return (0);
}
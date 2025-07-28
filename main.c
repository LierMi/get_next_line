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
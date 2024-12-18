#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

void	test_file(const char *filename)
{
	int		fd;
	char	*line;
	int		line_count;

	printf("\nTesting file: %s\n", filename);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		printf("Error opening file %s\n", filename);
		return ;
	}

	line_count = 1;
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("Line %d: %s", line_count++, line);
		free(line);
	}
	close(fd);
}

void	test_invalid_fd(void)
{
	char	*line;

	printf("\nTesting invalid fd (-1):\n");
	line = get_next_line(-1);
	if (line == NULL)
		printf("Test passed: Returns NULL for invalid fd\n");
	else
	{
		printf("Test failed: Should return NULL for invalid fd\n");
		free(line);
	}
}

void	create_test_file(const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		printf("Error creating test file\n");
		return ;
	}
	write(fd, "First line\n", 11);
	write(fd, "Second line\n", 12);
	write(fd, "Third line without newline", 25);
	close(fd);
}

int	main(void)
{
	const char	*test_filename = "test.txt";

	printf("Starting get_next_line tests...\n");
	create_test_file(test_filename);
	test_file(test_filename);
	test_file("test2.txt");
	// test_file("bible.txt");

	// Test invalid file descriptor
	test_invalid_fd();

	// Clean up
	unlink(test_filename);
	return (0);
}

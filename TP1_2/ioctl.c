#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#define IO_CRYPT _IOW('b', 1, int)


int main(int argc, const char **argv)
{
	// Files
	const char *name = "/dev/blkd";
	int err;
    char passwd[24] = "tarasseboulba";

	printf("USAGE : ./ioctl <optional passwd 24 charaters max> \n");
	printf("INFO : IF NO ARGUMENT PASSWD SET AS \"tarasseboulba\" \n");

    if (argc == 2)
		strcpy(passwd, argv[1]);


	// Try open the file
	int fd  = open(name, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "[main] ERROR : CANNOT OPEN -> '%s'\n", name);
		return EXIT_FAILURE;
	}

	// Try to send ioctl to clear this process memory
	err = ioctl(fd, IO_CRYPT, passwd);
	if (err) {
		fprintf(stderr, "[%s] ERROR IOCTL \n", name);
	} else {
		printf("[%s] SUCCESS IOCTL \n", name);
	}

	// Close
	close(fd);

	return EXIT_SUCCESS;
}

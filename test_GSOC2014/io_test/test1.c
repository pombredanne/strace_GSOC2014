#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFSIZE (100)

#define READFILE "test1_readfile"
#define WRITEFILE "test1_writefile"

int main(int argc, char *argv[])
{
    int fd_from;
    int fd_to;

    fd_from = open(READFILE, O_RDONLY);
    if ( fd_from == -1 ) {
        fprintf(stderr, "failed to open %s\n", READFILE);
        return -1;
    }

    fd_to = open(WRITEFILE, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if ( fd_to == -1 ) {
        fprintf(stderr, "failed to open output file:%s\n", WRITEFILE);
		return -1;
    }

    char buf[BUFSIZE];
    int n;
    while (n = read(fd_from, buf, BUFSIZE)) {
        if (n == -1) {
            fprintf(stderr, "failed read from %s\n", READFILE);
            return -1;
        }
        n = write(fd_to, buf, n);
        if ( n == -1 ) {
            fprintf(stderr, "failed to write to %s\n", WRITEFILE);
            return -1;
        }
    }
    
    printf("exited\n");
    return 0;
}

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <linux/limits.h>

#define ERR(source)(fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))

void remove_fifo(const char *pathname)
{
    if(unlink(pathname) == -1)
    {
        if(errno == ENOENT) return; // nothing to unlink
        ERR("unlink");
    }
}

void create_fifo(const char *pathname,const mode_t mode)
{
    remove_fifo(pathname);
    if(mkfifo(pathname,mode))
    {
        ERR("mkfifo");
    }
}

int open_file(const char* filename,const int flags)
{
    int fd = open(filename,flags);
    if(fd == -1)
        ERR("open");
    return fd;
}

void close_file(const int fd)
{
    if(close(fd) == -1)
        ERR("close");
}

void read_from_fifo(const int fd)
{
    ssize_t ret;
    char buffer[PIPE_BUF];
    do
    {
        ret = read(fd,buffer,1);
        if(ret < 0) ERR("read");
        if(ret > 0)
            for(int i=0; i < ret; i++)
                if(isalnum(buffer[i])) putchar(buffer[i]);
    } while(ret > 0);
    putchar('\n');
}

void usage(const char *progname)
{
    fprintf(stderr, "Usage: %s fifo_name\n",progname);
    exit(EXIT_FAILURE);
}

char *read_args(int argc,char **argv)
{
    if(argc!=2)
        usage(argv[0]);
    return argv[1];
}

int main(int argc,char **argv)
{
    char* pathname = read_args(argc,argv);
    create_fifo(pathname,0666);
    int fd = open_file(pathname,O_RDONLY);
    read_from_fifo(fd);
    close_file(fd);
    remove_fifo(pathname);
    return EXIT_SUCCESS;
}
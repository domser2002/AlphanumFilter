#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/limits.h>
#include <string.h>
#define ERR(source)(fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))
#define MSG_SIZE (PIPE_BUF-sizeof(pid_t))

int open_file(const char* filename,const int flags)
{
    printf("%s\n",filename);
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

int read_segment_from_fd(const int fd,char *dest, const int segment_size)
{
    int ret = read(fd,dest,segment_size);
    if(ret == -1)
        ERR("read");
    if(ret < segment_size)
        memset(dest + ret,0,segment_size-ret);
    return ret;
}

void write_to_fifo(const int fifo_fd, const int file_fd)
{
    while(1)
    {
        char* buf = malloc(MSG_SIZE*sizeof(char));
        int ret = read_segment_from_fd(file_fd,buf,MSG_SIZE);
        if(ret == 0) break;
        char* buffer = malloc(PIPE_BUF*sizeof(char));
        buffer[0] = '\0';
        char* pid = malloc(sizeof(pid_t) * sizeof(char));
        sprintf(pid,"%d",getpid());
        strcat(buffer,pid);
        strcat(buffer,buf);
        if(write(fifo_fd,buffer,PIPE_BUF) == -1)
            ERR("write");
    }
}

int main(int argc,char **argv)
{
    if(argc != 3)
        return EXIT_FAILURE;
    const char* filename = argv[1];
    const char* fifoname = argv[2];
    int fifofd,filefd;
    fifofd = open_file(fifoname,O_WRONLY);
    filefd = open_file(filename,O_RDONLY);
    write_to_fifo(fifofd,filefd);
    close_file(fifofd);
    close_file(filefd);
    return EXIT_SUCCESS;
}
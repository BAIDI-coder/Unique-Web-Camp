#include "Socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//显示出错信息，结束进程
void err_sys(const char* msg)
{
    time_t now = time(NULL);
    FILE *err_stream;
    if(err_stream = fopen("./error.log", "a"))
    fprintf(err_stream, "errno = %s %s %ld\n", strerror(errno), msg, now);
    exit(1);
}

//尽量输入n bytes，返回输入的字节数，异常则结束进程
__ssize_t Write(int fd, const void *buf, size_t n)
{
    int write_n;
    __ssize_t remain = n; //剩余带输入的字节数
    const void *ptr_buf = buf;
    while(remain > 0){
        if( (write_n = write(fd, ptr_buf, remain)) < 0){
            if(errno == EINTR) continue;
            err_sys("fail to write!");
            exit(1);
        }
        remain -= write_n;
        ptr_buf += write_n;
    }
    return n - remain;
}
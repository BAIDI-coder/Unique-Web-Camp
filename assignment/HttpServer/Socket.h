/*
    包含socket编程的必要头文件与宏
*/

//头文件
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

//宏
#define SA struct sockaddr
#define LISTENQ 5
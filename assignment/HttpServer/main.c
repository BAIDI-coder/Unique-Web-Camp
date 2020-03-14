#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Socket.h"
#include "Wrapper.hpp"
#define MAXBUFF 100
#define MAXURL 50
#define MAXPATH 50
#define MAXLINE 100

int serv_todo(int conn_fd); //返回0代表处理成功
int peer_request(char *buff, char* arr);    //从缓冲buff读取一行，返回读取字节数，失败则返回-1
int file_trans(int fd, const char *url);    //根据URL使用fd传送文件
int response404(int fd);  //404响应

int main(int argc, char **argv)
{
    int listen_fd, conn_fd;   //监听套接字（尚未监听）,已连接套接字（尚未连接）
    struct sockaddr_in serv_addr, cli_addr; //服务端&客户端地址结构(IPv4)
    pid_t pid; //进程号
    socklen_t cli_len = sizeof(cli_addr), serv_len = sizeof(serv_addr);  //客户端&服务端地址结构长度

    if( (listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("fail to create a socketfd!");

    memset(&serv_addr, 0, serv_len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(80);     //填充地址结构
    
    if(bind(listen_fd, (SA*)&serv_addr, serv_len) < 0){
        err_sys("fail to bind a socketfd!");
    }
    if(listen(listen_fd, LISTENQ) < 0)
        err_sys("fail to listen!");     //绑定与监听

    for(;;){   //TODO
        if( (conn_fd = accept(listen_fd, (SA*)&cli_addr, &cli_len)) < 0){
            if(errno == EINTR)  continue;
            else exit(1);
        }
        //printf("success!");
        if( (pid = fork()) == 0){
            close(listen_fd);
            if(serv_todo(conn_fd)) err_sys("fail to do!");
            exit(0);
        }
        else if(pid < 0) err_sys("fail to fork!");    //TODO
        close(conn_fd);    //父进程关闭连接
    }   //连接与处理

    return 0;
}

int serv_todo(int conn_fd)  //返回0代表处理成功,1代表异常
{
    char read_content[100];
    char read_buff[MAXBUFF];
    int read_n;  //读取数目
    memset(read_buff, 0, sizeof(read_buff));    //初始化缓冲区
    for(;;){
        if( (read_n = read(conn_fd, read_buff, MAXBUFF) < 0)){
            if(errno == EINTR) continue;
            else return 1;  
        }
        else if(!read_n) break;
    }   //更新进程缓冲区
    if(!peer_request(read_buff, read_content))   //读取报文请求行,不改变缓冲区file offset
        err_sys("fail to peer request!");
    
    char url[MAXURL];
    char method[10];   //http请求方法
    char version[10];  //http version
    if(sscanf(read_content, "%s%s%s", method, url, version) == 3){
        printf("\nsuccess: %s %s %s\n", method, url, version);   //Debug
        if(strcmp(method, "GET"))   return 1;   //不是GET方法
        if(file_trans(conn_fd, url)) puts("\nfail to FTS!"); //Debug
    }
    else return 1;  //请求行格式异常
    printf("\n%s\n", read_content);    //Debug 查看请求行
    return 0;
}

int peer_request(char *buff, char* arr)
{
    char *ptr_read = arr;
    char *ptr_buff = buff;
    int num = 0;
    for(;;){
        if(*ptr_buff != '\n' && *ptr_buff != '\0'){
            *ptr_read++ = *ptr_buff++;
            num++;
        }
        else{
            *ptr_read = '\0';
            break;
        }
    }
}

int file_trans(int fd, const char *url) //传输文件，返回0代表成功
{
    char file_path[MAXPATH] = "wwwroot";    //TODO: 有越界风险！
    strcat(file_path, url);
    struct stat file_stat;
    if(stat(file_path, &file_stat)){
        response404(fd);
            printf("\nnot found: %s\n", file_path);  //Debug
        return 1;
    }   //文件不存在

    if(S_ISDIR(file_stat.st_mode))
    {
        strcat(file_path, "index.html");

        if(stat(file_path, &file_stat)){
            response404(fd);
            printf("\nindex no: %s\n", file_path);  //Debug
            return 1;
        }   //文件不存在
    }    //目录下打开主页面

    printf("\nfile_path: %s\n", file_path);  //Debug
    //编写响应头
    char *(header[10]);
    header[0] = "HTTP/1.1 200 OK\n";
    header[1] = "Content-Type: text/html";
    header[2] = ";charset=utf-8\n";
    header[3] = "Server: BAIDIServ\n";
    header[4] = "\n";
    
    for (int i = 0; i < 5; i++) Write(fd, header[i], strlen(header[i]));
    
    //文件传输
    int responsefd;
    if( (responsefd = open(file_path, O_RDONLY)) < 0) err_sys("fail to open file!");
    if(sendfile(fd, responsefd, NULL, file_stat.st_size) == -1){
        err_sys("fail to send file!");
    }
    close(responsefd);  //!记得开了要关！！
    puts("leave from FTS!");    //Debug
    return 0;
}

/*int file_trans(int fd, const char *url) //传输文件，返回0代表成功
{
    int flag;   //是否存在文件
    FILE *fptr = NULL;
    if( (fptr = fopen(url, "r")) == NULL) flag = 0; //文件不存在或无法打开

    char *write_buff[MAXBUFF];  //缓冲机制
    char *ptr_buff = write_buff;
    while( (fgets(ptr_buff, MAXLINE, fptr))){
        ptr_buff = strlen(ptr_buff);    //更新缓冲区指针
    }
    int remain = (int)(ptr_buff - write_buff);

    ptr_buff = write_buff;
    int write_n;
    while(remain > 0){
        if( (write_n = write(fd, ptr_buff, MAXBUFF)) < 0){
            if(errno == EINTR) continue;
            err_sys("fail to write!");
        }
        
        remain -= write_n;
        ptr_buff += write_n;
    }
}*/     //fopen实现，速度慢，要经过内核-进程-内核

int response404(int fd)  //404响应
{
    char *msg = "HTTP/1.1 404 not found";
    Write(fd, msg, strlen(msg));
    char *page404 = "\n\n404 not found!";
    Write(fd, page404, strlen(page404));
    return 0;
}
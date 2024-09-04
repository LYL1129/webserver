#include<stdio.h>
#include<libgen.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<assert.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char* argv[]){

    if(argc<=2){
        printf("usage: %s, ip_adress, port_number\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family=AF_INET;
    address.sin_port= htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    //创建socket
    int sock = socket(PF_INET, SOCK_STREAM, 0);

    assert(sock>0);
    //绑定服务器socket
    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));

    assert(ret!=-1);
    /*listen 本地已建立的socket套接字，b
    acklog表示请求队列的最大长度，用于限制排队请求的个数，
    maxvalue=5，如果没有错误发生返回0*/
    ret = listen(sock, 5);
    assert(ret!=-1);
    
    struct sockaddr_in client_addr;
    socklen_t client_addrlength = sizeof(client_addr);

    int connfd = accept(sock, (struct sockaddr*)&client_addr, &client_addrlength);

    if(connfd<0){
        printf("errno is: %d\n", errno);

    }else{
        //关闭标准文件描述符STDOUT_FILENO
        close(STDOUT_FILENO);
        //复制socket文件描述符connfd，dup返回系统中最小的fd，实际为1
        dup(connfd);
        printf("abcd\n");
        close(connfd);
    }

    close(sock);

    return 0;
}
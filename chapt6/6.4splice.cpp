#include<stdio.h>
#include<libgen.h>
#include<sys/socket.h>
#include<stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<errno.h>
int main(int argc, char* argv[]){

    if(argc<=2){
        printf("usage: %s ip_address port_number\n ", basename(argv[0]));
    }

    const char* ip = argv[1];
    // 将字符串转为数字
    int port = atoi(argv[2]);
    struct sockaddr_in address;
    memset(&address, 0,sizeof(address));
    address.sin_family=AF_INET;
    address.sin_port=htons(port);

    inet_pton(AF_INET, ip, &address.sin_addr);
    //创建服务器socket
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock>=0);

    //绑定ip和端口
    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret!=-1);
    if(ret<0){
        perror("bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    ret=listen(sock, 5);
    assert(ret!=-1);
    //创建客户端sock地址
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    //等待客户端socket连接
    int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);

    if(connfd<0){
        printf("errno is: %d\n", errno);
    }else{
        
    }
    return 0;
}


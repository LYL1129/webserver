#include<cstdio>
#include<libgen.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<cstring>

#include <arpa/inet.h>

#include<assert.h>
#define BUFFER_size 512

int main(int argc, char* argv[]){


    if(argc<=2){
         printf("usage: %s ip_address port_number send_buffer_size\n", basename(argv[0]));
         return 1;
    }

    //定义客户端ip地址
    const char*  ip= argv[1];
    int port = atoi(argv[2]);
    
    struct sockaddr_in server_address; //arpa/inet.h
    memset(&server_address, 0, sizeof(server_address) );

    server_address.sin_family=AF_INET; //IPv4 Internet protocols.
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    
    assert(sockfd>0);
    int sendbuf = atoi(argv[3]); //将字符串转为int值
    int len = sizeof(sendbuf);
    // 先设置TCP发送缓冲区大小，然后立即读取
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof(sendbuf));
    getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuf, (socklen_t*)&len);
    
    printf("the tcp send buffer size after setting is %d\n", sendbuf);
    if(connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) != -1 ){
        char buffer[BUFFER_size];
        memset(buffer, 'a', BUFFER_size);
        send(sockfd, buffer, BUFFER_size, 0);
    }

    close(sockfd);
    






    return 0;
}
        #include<sys/socket.h>
#include<cstdio>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<errno.h>
#include<libgen.h>
#define BUFFER_SIZE 1024
int main(int argc, char* argv[]){
    if(argc<=2){
        printf("usage: %s ip_address port_number rece_buffer_size\n", basename(argv[0]));
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]); //stdlib

 // 初始化 sockaddr_in 结构体
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));

    address.sin_family=AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    // address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    char ip_str[INET_ADDRSTRLEN]; // Buffer to hold the IP address string
    // Convert the IP address to a string representation
    if (inet_ntop(AF_INET, &(address.sin_addr), ip_str, sizeof(ip_str)) == NULL) {
        perror("inet_ntop");
        return 1;
    }

    
    
    // 创建套接字
    int client_sockfd= socket(PF_INET, SOCK_STREAM, 0);
    assert(client_sockfd>=0);
    

    // int recvbuf = atoi(argv[3]);
    // int len = sizeof(recvbuf);

    // setsockopt(client_sockfd, SOL_SOCKET, SO_RCVBUF, &recvbuf, sizeof(recvbuf));
    // getsockopt(client_sockfd, SOL_SOCKET, SO_RCVBUF, &recvbuf, (socklen_t*)&len);

// 绑定套接字
    int ret = bind(client_sockfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret!=-1);
    ret = listen(client_sockfd, 5);
    assert(ret!=-1);
    
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);

    int connfd = accept(client_sockfd, (struct sockaddr*)&client, &client_addrlength);

    ssize_t bytes_received;

    if(connfd<0){
        printf("errno is: %d\n", errno);

    }else{
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));
        while(bytes_received  = recv(connfd, buffer, BUFFER_SIZE-1, 0) > 0){
            buffer[bytes_received] = '\0';  // Null-terminate the received data
            printf("Received from client: %s\n", buffer);

             // Echo back the message to the client
            if (send(client_sockfd, buffer, bytes_received, 0) < 0) {
                perror("send error");
                break;
            }
        }
        close(connfd);

    }

    close(client_sockfd);
    return 0;

}



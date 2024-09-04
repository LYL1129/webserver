#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<cstring>
#include<netdb.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 128
int main(int argc, char* argv[]){
    // assert(argc==2);
    
    char* host = argv[1];
    int port = atoi(argv[2]);
    printf("server port is: %s\n", argv[2]);

    //根据主机名获取主机完整信息
    struct hostent* hostinfo = gethostbyname(host);
    assert(hostinfo);
    printf("the name of host: %s\n", hostinfo->h_name);
    printf("the aliases of host: %p\n", hostinfo->h_aliases);
    printf("the addr_family: %d\n", hostinfo->h_addrtype);
    printf("the address length: %d\n", hostinfo->h_length);
    printf("Addresses:\n");
    for (char **addr = hostinfo->h_addr_list; *addr != NULL; addr++) {
        char ip[INET_ADDRSTRLEN]; // Size for IPv4 addresses
        inet_ntop(AF_INET, *addr, ip, sizeof(ip));
        printf("  %s\n", ip);
    }


    // 获取daytime服务信息
    struct servent* servinfo = getservbyname("daytime", "tcp");
    assert(servinfo);
    printf("daytime port is %d\n", ntohs(servinfo->s_port));
    printf("Aliases:\n");

    for (char **alias = servinfo->s_aliases; *alias != NULL; alias++) {
        printf("  %s\n", *alias);
    }
    printf("protocol: %s\n", servinfo->s_proto);


    struct sockaddr_in address;
    address.sin_family=AF_INET;
    address.sin_port = htons(port);
    /*因为h本身就是使用网络字节序的地址列表，所以使用其中的IP地址时，无需转换字节序 */
    address.sin_addr=*(struct in_addr*)*hostinfo->h_addr_list;
 
    struct sockaddr_in sa;
    char ip_str[INET_ADDRSTRLEN];
    if( inet_ntop(AF_INET, &(address.sin_addr), ip_str, sizeof(ip_str))==NULL){
        perror("inet_ntop");
        return 1;
    }

    // Print the IP address
    printf("The IP address is: %s\n", ip_str);

    int sockfd = socket(PF_INET, SOCK_STREAM,0);
     if (sockfd < 0) {
        perror("create socket failed! ");
        return 1;
    }

    char buffer[BUFFER_SIZE];

    // int send_buffer = atoi(argv[3]);
    // int send_len = sizeof(send_buffer);

    // setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &send_buffer, sizeof(send_buffer));
    // getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF,&send_buffer, (socklen_t*)&send_buffer );

    int result = connect(sockfd, (struct sockaddr*) &address, sizeof(address));
    if (result < 0) {
        perror("Connect"); //如果出现连接错误，那么可能是端口错误
        close(sockfd);
        return 1;
    }    
    
    ssize_t bytes_received;
    

    memset(buffer, 'a', BUFFER_SIZE);
    bytes_received = send(sockfd, buffer, BUFFER_SIZE, 0);
    
    if(bytes_received<0){
        perror("recv error");
    } 
   

    

    buffer[bytes_received] = '\0';
    printf("the daytime is: %s\n", buffer);

    close(sockfd);

    return 0;
}
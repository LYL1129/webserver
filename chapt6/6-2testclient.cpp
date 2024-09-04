#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>
#include <unistd.h>
#define BUFFER_SIZE 128
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: %s <ip_address> <port_number>\n", argv[0]);
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    // 创建 socket
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    // 连接到服务器
    int ret = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("connect failed");
        close(sock);
        return 1;
    }

    // 发送数据到服务器
     char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';  // Null-terminate the received data
        printf("Received from client: %s\n", buffer);
        
        // Echo back the message to the client
        if (send(sock, buffer, bytes_received, 0) < 0) {
            perror("send error");
            break;
        }
    }

    printf("not recv");
    // 关闭 socket
    close(sock);

    return 0;
}

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        std::cout << "ERROR" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    int bindSuccess = bind(sockfd, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr));

    if(bindSuccess < 0){
        std::cout << "ERROR BINDING SERVER" << std::endl;
        return 1;
    }

    if( listen(sockfd, 5) < 0){
        std::cout << "ERROR LISTENING" << std::endl;
        return 1;
    }
    std::cout << "Waiting for client..." << std::endl;

    sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    socklen_t addrLen = sizeof(clientAddr);
    int client_fd = accept(sockfd,reinterpret_cast<struct sockaddr*>(&clientAddr), &addrLen);
    if(client_fd < 0){
        std::cout << "ERROR ACCEPTING" << std::endl;
        return 1;
    }
    std::cout << "Client connected to server" << std::endl;

    char clientMessage[1024];
    char serverMessage[1024] = {'H', 'e', 'l', 'l', 'o', '\n'};
    bool running = 1;
    while(running){
        int bytesRec = recv(client_fd, clientMessage, sizeof(clientMessage), 0);
        if(bytesRec < 0){
            std::cout << "ERROR RECIEVING MESSAGE" << std::endl;
            return 1;
        }
        if(bytesRec == 0){
            std::cout << "USER DISCONNECTED" << std::endl;
            running = 0;
        }
        clientMessage[bytesRec] = '\0';
        for(int i = 0; i < bytesRec; i++){
            std::cout << clientMessage[i];
        }
        int serverStrLen = strlen(serverMessage);
        serverMessage[serverStrLen] = '\0';
        if(bytesRec > 0){
            send(client_fd, serverMessage, serverStrLen, 0);
            for(size_t i = 0; i < serverStrLen; i++){
                std::cout << serverMessage[i];
            }
        }
    }
    return 0;
}
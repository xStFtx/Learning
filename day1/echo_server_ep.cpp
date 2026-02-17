#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <cstring>
#include <fcntl.h>

constexpr int MAX_EVENTS = 10;
constexpr int MAX_CLIENTS = 10;
constexpr int PORT = 6379;

//Handle client connections;
void handleClient(int clientFd) {
    char buffer[1024];

    while(true) {
        int bytesRead = read(clientFd, buffer, sizeof(buffer) - 1);
        if (bytesRead <= 0) {
            // Client disconnected or error
            break;
        }

        buffer[bytesRead] = '\0'; // Null-terminate, just in case

        const char *response = "+PONG\r\n";
        // Send full response length
        write(clientFd, response, strlen(response));
    }

    close(clientFd);
}

int main() {
    int serverFd, epollFd;
    struct sockaddr_in serverAddress;
    struct epoll_event event, events[MAX_EVENTS];
    //Create socket
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    int flags = fcntl(serverFd, F_GETFL, 0);
    fcntl(serverFd, F_SETFL, flags | O_NONBLOCK);
    if (serverFd < 0) {
        std::cerr <<  "Failed to create socket." << std::endl;
        return 1;
    }

    serverAddress.sin_family =AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    //Bind socket to address and port
    if (bind(serverFd,(struct sockaddr*) &serverAddress,sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(serverFd);
        return 1;
    }

    //Listen for incoming connections
    if (listen(serverFd, MAX_CLIENTS) == -1) {
        std::cerr <<"Failed to listen. " << std::endl;
        close(serverFd);
        return 1;
    }

    //Create epoll instance
    epollFd = epoll_create1(0);
    if (epollFd == -1) {
        std::cerr << "Failed to create epoll instance" << std::endl;
        close(serverFd);
        return 1;
    }

    //Add server socket to epoll
    event.events = EPOLLIN;
    event.data.fd = serverFd;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFd, &event) == -1){
        std::cerr << "Failed to add server socket to epoll instance." << std::endl;
        close(serverFd);
        close(epollFd);
        return 1;
    } 

    std::cout << "Server started. Listening to port " << PORT << std::endl;

    while (true) {
        int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (numEvents == -1){
            std::cerr << "Failed to wait for events." << std::endl;
            break;
        }
        for (int i =0; i < numEvents; ++i) {
            if (events[i].data.fd == serverFd) {
                // Accept new client connection
                struct sockaddr_in clientAddress;
                socklen_t clientAddressLength = sizeof(clientAddress);
                int clientFd = accept(serverFd, (struct sockaddr*) &clientAddress, &clientAddressLength);
                int flags = fcntl(clientFd, F_GETFL, 0);
                fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);

                if (clientFd == -1) {
                    std::cerr << "Failed to accept client connection." << std::endl;
                    continue;
                }

                //Add client socket to epoll
                event.events = EPOLLIN;
                event.data.fd = clientFd;
                if (epoll_ctl(epollFd, EPOLL_CTL_ADD,clientFd,&event) == -1) {
                    std::cerr << "Failed to add client socket to epoll instance" << std::endl;
                    close(clientFd);
                    continue;
                }
            }else {
                int clientFd = events[i].data.fd;

                char buffer[1024];
                int bytesRead = read(clientFd, buffer, sizeof(buffer));

                if (bytesRead <= 0) {
                    close(clientFd);
                    epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, nullptr);
                } else {
                    const char* response = "+PONG\r\n";
                    write(clientFd, response, strlen(response));
                }
            }
        }

    }

    close(serverFd);
    close(epollFd);


    return 0;
}
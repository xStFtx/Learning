#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <map>
#include <string>

constexpr int MAX_EVENTS = 10;
constexpr int MAX_CLIENTS = 10;
constexpr int PORT = 6379;

struct MessageCounting {
    sockaddr_in addr;
    int message_count;
};

int main() {
    int serverFd, epollFd;
    struct sockaddr_in serverAddress{};
    struct epoll_event event, events[MAX_EVENTS];
    std::map<int, MessageCounting> clients;

    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        std::cerr << "Failed to create socket.\n";
        return 1;
    }

    // Allow quick restart
    int reuse = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // Make server socket non-blocking
    int flags = fcntl(serverFd, F_GETFL, 0);
    fcntl(serverFd, F_SETFL, flags | O_NONBLOCK);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Bind failed\n";
        close(serverFd);
        return 1;
    }

    if (listen(serverFd, MAX_CLIENTS) < 0) {
        std::cerr << "Listen failed\n";
        close(serverFd);
        return 1;
    }

    epollFd = epoll_create1(0);
    if (epollFd == -1) {
        std::cerr << "Failed to create epoll instance\n";
        close(serverFd);
        return 1;
    }

    event.events = EPOLLIN | EPOLLET;
    event.data.fd = serverFd;

    epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFd, &event);

    std::cout << "Server started on port " << PORT << "\n";

    while (true) {
        int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (numEvents == -1) {
            std::cerr << "epoll_wait failed\n";
            break;
        }        
        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == serverFd) {
                while (true) {
                    sockaddr_in clientAddr{};
                    socklen_t clientLen = sizeof(clientAddr);
                    int clientFd = accept(serverFd,(sockaddr*)&clientAddr,&clientLen);
                    //Client IP
                    if(clientFd > 0) {
                        MessageCounting mc;
                        mc.addr = clientAddr;
                        mc.message_count = 0;
                        clients[clientFd] = mc;

                        char ipStr[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &clientAddr.sin_addr,ipStr, sizeof(ipStr));
                        std::cout<<"New connection from " << ipStr << ":" << ntohs(clientAddr.sin_port) <<"\n";
                    }
                    if (clientFd == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        else {
                            perror("accept");
                            break;
                        }
                    }
                    // Make client non-blocking
                    int cflags = fcntl(clientFd, F_GETFL, 0);
                    fcntl(clientFd, F_SETFL, cflags | O_NONBLOCK);

                    event.events = EPOLLIN | EPOLLET;
                    event.data.fd = clientFd;
                    epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event);
                }
            } else {
                while (true) {
                    int clientFd = events[i].data.fd;
                    char buffer[1024];
                    int bytesRead = read(clientFd, buffer, sizeof(buffer));
                    if (bytesRead > 0) {
                        clients[clientFd].message_count++;
                        char ipStr[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &clients[clientFd].addr.sin_addr, ipStr, sizeof(ipStr));
                        std::cout << "[" << ipStr << ":" << ntohs(clients[clientFd].addr.sin_port) << "] Message #" << clients[clientFd].message_count << "\n";
                        const char* response = "+PONG\r\n";
                        write(clientFd, response, strlen(response));
                    }
                    else if (bytesRead == 0) {
                        // Client disconnected
                        close(clientFd);
                        epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, nullptr);
                        clients.erase(clientFd);
                        break;
                    }
                    else {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break; 
                        } else {
                            close(clientFd);
                            epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, nullptr);
                            break;
                        }
                    }
                }
            }
        }
    }

    close(serverFd);
    close(epollFd);
    return 0;
}

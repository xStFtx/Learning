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
#include <string>
#include <algorithm>
#include <unordered_map>
#include <functional>

constexpr int MAX_EVENTS = 10;
constexpr int MAX_CLIENTS = 10;
constexpr int PORT = 6379;

struct ClientInfo { sockaddr_in addr; int message_count; std::string buffer;};

using CommandHandler = std::function<std::string(const std::string&, ClientInfo&)>;

struct CommandEntry {
    std::string description;
    CommandHandler handler;
};

std::string ping_c(const std::string&, ClientInfo&) {
    return "+PONG\r\n";
}

std::string count_c(const std::string&, ClientInfo& client) {
    return "Count: " + std::to_string(client.message_count) + "\r\n";
}

std::string echo_c(const std::string& args, ClientInfo&) {
    return args + "\r\n";
}

std::unordered_map<std::string, CommandEntry> command_table = {
    {"PING",  {"Return PONG", ping_c}},
    {"COUNT", {"Return message count", count_c}},
    {"ECHO",  {"Echo message", echo_c}}
};

std::string dispatch(std::string input, ClientInfo& client) {
    while (!input.empty() && (input.back() == '\n' || input.back() == '\r'))
        input.pop_back();

    std::string command;
    std::string args;
    size_t spacePos = input.find(' ');
    if (spacePos != std::string::npos) {
        command = input.substr(0, spacePos);
        args = input.substr(spacePos + 1);
    } else {
        command = input;
    }
    std::transform(command.begin(), command.end(),command.begin(), ::toupper);

    auto it = command_table.find(command);
    if (it != command_table.end()) {
        return it->second.handler(args, client);
    }

    return "-ERR unknown command\r\n";
}


int main() {
    int serverFd, epollFd;
    sockaddr_in serverAddress{};
    epoll_event event{}, events[MAX_EVENTS];
    std::unordered_map<int, ClientInfo> clients;
    std::unordered_map<std::string, CommandEntry> command_table;
    // Create socket
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        perror("socket");
        return 1;
    }

    // Allow quick restart
    int reuse = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // Make server non-blocking
    int flags = fcntl(serverFd, F_GETFL, 0);
    fcntl(serverFd, F_SETFL, flags | O_NONBLOCK);

    // Setup address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverFd, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("bind");
        close(serverFd);
        return 1;
    }

    if (listen(serverFd, MAX_CLIENTS) < 0) {
        perror("listen");
        close(serverFd);
        return 1;
    }

    // Create epoll instance
    epollFd = epoll_create1(0);
    if (epollFd == -1) {
        perror("epoll_create1");
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
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == serverFd) {
                while (true) {
                    sockaddr_in clientAddr{};
                    socklen_t clientLen = sizeof(clientAddr);
                    int clientFd = accept(serverFd,(sockaddr*)&clientAddr,&clientLen);
                    if (clientFd == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        perror("accept");
                        break;
                    }
                    // Make client non-blocking
                    int cflags = fcntl(clientFd, F_GETFL, 0);
                    fcntl(clientFd, F_SETFL, cflags | O_NONBLOCK);
                    // Store client info
                    clients[clientFd] = {clientAddr, 0};
                    char ipStr[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &clientAddr.sin_addr,ipStr, sizeof(ipStr));
                    std::cout << "New connection from "<< ipStr << ":"<< ntohs(clientAddr.sin_port) << "\n";

                    // Add to epoll
                    event.events = EPOLLIN | EPOLLET;
                    event.data.fd = clientFd;
                    epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event);
                }
            }

            else {
                int clientFd = events[i].data.fd;
                while (true) {
                    char buffer[1024];
                    int bytesRead = read(clientFd, buffer, sizeof(buffer));
                    if (bytesRead > 0) {
                        clients[clientFd].buffer.append(buffer, bytesRead);

                        char ipStr[INET_ADDRSTRLEN];
                        inet_ntop(AF_INET, &clients[clientFd].addr.sin_addr, ipStr, sizeof(ipStr));
                        std::cout << "[" << ipStr << ":" << ntohs(clients[clientFd].addr.sin_port) << "] Message #" << clients[clientFd].message_count << "\n";

                        // Process complete lines
                        size_t pos;
                        while (true) {
                            size_t pos_rn = clients[clientFd].buffer.find("\r\n");
                            size_t pos_n  = clients[clientFd].buffer.find("\n");

                            if (pos_rn != std::string::npos && (pos_n == std::string::npos || pos_rn < pos_n)) {
                                pos = pos_rn;
                            } else if (pos_n != std::string::npos) {
                                pos = pos_n;
                            } else {
                                break; // no complete line yet
                            }
                            std::string line = clients[clientFd].buffer.substr(0, pos);
                            // Remove the line + correct newline length
                            clients[clientFd].buffer.erase(0, pos + ((pos_rn != std::string::npos && pos_rn == pos) ? 2 : 1));

                            clients[clientFd].message_count++;
                            std::string response = dispatch(line, clients[clientFd]);
                            write(clientFd, response.c_str(), response.size());
                        }
                    } else if (bytesRead == 0) {
                        // Client disconnected
                        close(clientFd);
                        epoll_ctl(epollFd,EPOLL_CTL_DEL,clientFd,nullptr);
                        clients.erase(clientFd);
                        break;
                    }else {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;

                        close(clientFd);
                        epoll_ctl(epollFd,EPOLL_CTL_DEL,clientFd,nullptr);
                        clients.erase(clientFd);
                        break;
                    }
                }
            }
        }
    }

    close(serverFd);
    close(epollFd);
    return 0;
}

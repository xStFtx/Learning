#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <netinet/in.h>
#include <vector>

uint16_t PORT = 6379;

void handle_client(int client_fd) {
    int bytes_recieved;
    char buffer[1024] = {0};

    while (true) {
        bytes_recieved = recv(client_fd, buffer, sizeof(buffer) -1 , 0);
        if (bytes_recieved > 0) {
            buffer[bytes_recieved] = '\0';
            std::cout << "Message from client: " << buffer << std::endl;
        } else if (bytes_recieved == 0) {
            std::cout << "Client disconnected gracefully\n";
            break;
        }
        else {
            std::cerr << "recv failed\n";
            break;
        }

        const char *response = "+PONG\r\n";
        size_t total_sent =0;
        size_t len = strlen(response);

        while (total_sent < len) {
            ssize_t sent = send(client_fd, response+total_sent, len-total_sent,0);
            if (sent <= 0) break;
            total_sent+= sent;
        }
    }
    close(client_fd);
}

int main(int argc, char **argv) {

    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    //fd is file discriptor; comes from unix saying that everything is a file.
    // domain , type, protocol. Socket create a socket of type on domain using protocol. If protocol is zero one is chosen automatically.
    int server_fd = socket(AF_INET, SOCK_STREAM,0);
    if (server_fd < 0) {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    //Setting SO_REUSEADDR ensures that you don't run into 'Address already in use' errors.
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))<0) {
        std::cerr << "setsockopt failed \n";
        return 1;
    }

    //Struct of internet socket address
    /*     struct sockaddr_in
    {
        __SOCKADDR_COMMON (sin_);
        in_port_t sin_port;			/* Port number.  */
    //  struct in_addr sin_addr;		/* Internet address.  */

    /* Pad to size of `struct sockaddr'.  */
    //  unsigned char sin_zero[sizeof (struct sockaddr)
    //          - __SOCKADDR_COMMON_SIZE
    //           - sizeof (in_port_t)
    //          - sizeof (struct in_addr)];
    //  };  

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
        std::cerr << "Failed to bind to port " << PORT << "\n";
        return 1;
    }

    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0) {
        std::cerr << "listen failed\n";
        return 1;
    }

    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    std::cout << "Waiting for a client to connect...\n";

    std::cout << "Logs from your program will appear here!\n";

    while(true) {
        int client_fd = accept(server_fd, (struct sockaddr*) &client_addr, (socklen_t *) &client_addr_len);
        if (client_fd < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }
        std::thread t(handle_client, client_fd);
        t.detach();
    }

    close(server_fd);
    return 0;
}
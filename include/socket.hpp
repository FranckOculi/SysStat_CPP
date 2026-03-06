#ifndef __Socket__
#define __Socket__

#define SOCKET_FAMILY AF_UNIX
#define SOCKET_NAME "/tmp/sysstat.sock"
#define MAX_CONNECTION_REQUEST 1
#define BUFFER_SIZE 1024

#include "system.hpp"
#include "common.hpp"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <signal.h>
#include <sys/time.h>
#include <cstring>
#include <stdexcept>
#include <iostream>

class Socket {
    public :
        Socket() = default;

        ~Socket() {
            if (_FD != -1) {
               close(_FD);
            }
        };
        
        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        int sendMessage(const char message[]) {
            return send(_FD, message, strlen(message), 0);
        }

        int receiveMessage(System::system_stats* buffer) {
            return recv(_FD, buffer, sizeof(*buffer), 0);
        }

        void resetFD(int fd) {
            if (_FD != -1) {
                close(_FD);
            }

            Common::print_log(stdout, "Set FD with value : %d\n", fd);
            _FD = fd;
        }

        int getFD() {
            return _FD;
        }

        bool valid() {
            return _FD != -1;
        }

    protected :        
        void create() {
            /* Create local socket.  */
            _FD = socket(SOCKET_FAMILY, SOCK_STREAM, 0);
            if (_FD == -1) {
                throw std::runtime_error("Failed to create socket");
            }
        };

        void connect_to_socket_addr() {
            sockaddr_un socket_addr;
            int socket_addr_size = sizeof(socket_addr);

            /*
            * For portability clear the whole structure, since some implementations have 
            * additional (nonstandard) fields in the structure.
            */
            memset(&socket_addr, 0, socket_addr_size);

            /* Connect socket to socket address.  */
            socket_addr.sun_family = AF_UNIX;
            strncpy(socket_addr.sun_path, SOCKET_NAME, sizeof(socket_addr.sun_path) - 1);

            int connect_return_code = connect(_FD, (const struct sockaddr*) &socket_addr, socket_addr_size);
            if (connect_return_code == -1) {
                throw std::runtime_error("Client : The server is down.");
            }
        }

        void accept_client() {
            sockaddr_un socket_name;
            int socket_name_size = sizeof(socket_name);

            /*
            * For portability clear the whole structure, since some implementations have 
            * additional (nonstandard) fields in the structure.
            */
            memset(&socket_name, 0, socket_name_size);
            
            /* Bind socket to socket name.  */
            socket_name.sun_family = SOCKET_FAMILY;
            strncpy(socket_name.sun_path, SOCKET_NAME, sizeof(socket_name.sun_path) - 1);

            int bind_return_code = bind(_FD, (const sockaddr*) &socket_name, socket_name_size);
            if (bind_return_code == -1) {
                throw std::runtime_error("Server : Failed to bind socket FD and socket address");
            }

            /*
            * Prepare for accepting connections. The backlog size is set to 2.  
            * So while one request is being processed other request can be waiting.
            */
            if(listen(_FD, MAX_CONNECTION_REQUEST) == -1) {
                throw std::runtime_error("Server : Failed to accept connections");
            };

            Common::print_log(stdout, "Server : Waiting for new connection...\n");
        }

        int _FD = -1;
};

#endif
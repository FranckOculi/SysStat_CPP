#include "server.hpp"
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

volatile sig_atomic_t stop = 0;

void Server::cleanup() {
    Common::print_log(stdout, "Server : cleanup.\n");

    if (client_socket.getFD() != -1) {
        /* To force stopping recv. */
        shutdown(client_socket.getFD(), SHUT_RDWR);
        client_socket.resetFD(-1);
    }

    if (_FD != -1) {
        close(_FD);
        _FD = -1;
    }

    if (_callback) {
        _callback();
    }

    unlink(SOCKET_NAME);
}

void Server::handle_signal(int) {
    Common::print_log(stdout, "Server : Caught user request to close the connection.\n");
    stop = 1;
}

int Server::run() {
    signal(SIGTERM, Server::handle_signal);

    System system;
    fd_set readfds;

    /* This is the main loop for handling connections.  */
    while (!stop) {
        /*
        * We are using select() witch allows a program to monitor multiple file 
        * descriptors, waiting until one or more of the file descriptors become "ready".
        * 
        * select() can monitor only file descriptors numbers that are less than 
        * FD_SETSIZE (1024). Use poll() instead, if needed, witch do not suffer 
        * this limitation.
        * 
        * select() is used to wait for activity on the socket with a timeout. 
        * This allows the main loop to periodically regain control and check
        * whether a termination signal has been received.
        * 
        * Without select(), the program would remain blocked in accept() or recv()
        * functions preventing handle_signal() to stopping the server.
        */        
        FD_ZERO(&readfds);
        FD_SET(_FD, &readfds);
        int max_fd = _FD;

        /* If a client is aleady connected, monitor his file descriptor. */
        if (client_socket.getFD() != -1) {
            FD_SET(client_socket.getFD(), &readfds);
            if (client_socket.getFD() > max_fd) max_fd = client_socket.getFD();
        }

        int ready_fds = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (ready_fds == -1) {
            if (errno == EINTR) continue;
            throw std::runtime_error("Server : Failed to monitor multiple FD");
        }

        /* 
        * At this point -> connected_selected = number of file descriptors contained in
        * the three returned descriptor sets (that is, the total number of bits that are
        * set in readfds, writefds, exceptfds).
        *
        * We can test if a file descriptor is still present in a set.
        */
        if (FD_ISSET(_FD, &readfds)) {
            int newFD = accept(_FD, NULL, NULL);
            if (newFD == -1) {
                if (errno == EINTR) continue;
                throw std::runtime_error("Server : Failed to accept connection");
            }
            
            if (client_socket.valid()) {
                Common::print_log(stdout, "Server : New client connection rejected.\n");
                System::system_stats empty_stats = {};
                if (send(newFD, &empty_stats, sizeof(empty_stats), 0) == -1) {
                    Common::print_log(stderr, "Server : Failed to send response to rejected client.\n");
                }

                shutdown(newFD, SHUT_RDWR);
            } else {
                client_socket.resetFD(newFD);
                Common::print_log(stdout, "Server : Client connected.\n");
            }
        }

        /*
        * Handle incoming data from the connected client -> If there is a connected client 
        * and select() indicates data is available to read.
        */
        if (client_socket.getFD() != -1 && FD_ISSET(client_socket.getFD(), &readfds)) {
            char rec_buffer[BUFFER_SIZE];
            System::system_stats system_stats = {};

            int received_bytes = recv(client_socket.getFD(), rec_buffer, BUFFER_SIZE - 1, 0);

            if (received_bytes > 0) {
                rec_buffer[received_bytes] = '\0';
                Common::print_log(stdout, "Server : Message received : %s\n", rec_buffer);

                system_stats = system.system_infos();
                
                if (send(client_socket.getFD(), &system_stats, sizeof(system_stats), 0) == -1) {
                    Common::print_log(stderr, "Server : Failed to send response.\n");
                }
            } else if (received_bytes == 0) {
                Common::print_log(stdout, "Server : Client disconnected.\n");
                    client_socket.resetFD(-1);
            } else {
                if (errno != EINTR) {
                    Common::print_log(stderr, "Server : Failed to read received bytes.\n");
                    client_socket.resetFD(-1);
                }
            }
        }
    }

    return 0;
}
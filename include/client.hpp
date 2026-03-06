#ifndef __CLIENT__
#define __CLIENT__

#define MESSAGE "Ping!"

#include "socket.hpp"
#include "common.hpp"
#include <iostream>

class Client : public Socket {
    public :
        Client() {
            create();
            connect_to_socket_addr();
        };

        void call() {
            if (sendMessage(MESSAGE) == -1) {
                throw std::runtime_error("Client : Failed to send message");
            };
        };
        
        void get_data() {
            int received_bytes = receiveMessage(&current_buffer);
            if (received_bytes == -1) {
                throw std::runtime_error("Client : Failed to received bytes");
            } else if (received_bytes != sizeof(current_buffer)) {
                throw std::runtime_error("Client : Received incomplete data");
            }
        };
        
        void print_data() {
            if (first_call == true) {
                first_call = false;
            } else {
                Common::print_cpu(Common::calcul_cpu_active(&current_buffer, &prev_buffer));
            }

            Common::print_mem(Common::calcul_mem_active(&current_buffer));
            Common::print_uptime(current_buffer.uptime.hours, current_buffer.uptime.minutes);

            prev_buffer = current_buffer;
        };

    private :
        unsigned int first_call = true;
        System::system_stats prev_buffer = {};
        System::system_stats current_buffer = {};    
};

#endif
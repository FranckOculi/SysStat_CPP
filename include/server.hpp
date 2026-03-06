#ifndef __NETWORK__
#define __NETWORK__

#include "socket.hpp"

typedef void (*CleanupCallback)();

class Server : public Socket {
    public :
        Server(CleanupCallback cb) {
            _callback = cb;
            create();
            accept_client();
        }

        ~Server() {
            cleanup();
        }

        int run();

    private :
        void cleanup();
        static void handle_signal(int sig_number);
        CleanupCallback _callback;
        Socket client_socket;
};

#endif
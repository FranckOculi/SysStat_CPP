#ifndef __NETWORK__
#define __NETWORK__

class Network {
    public :
        int run(void close_log_file());

    private :
        static void handle_signal(int sig_number);
};

#endif
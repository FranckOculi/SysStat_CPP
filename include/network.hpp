#ifndef __NETWORK__
#define __NETWORK__

class Network {
    public :
    int run(void);
    
    private :
        static void handle_signal(int sig_number);
};

#endif
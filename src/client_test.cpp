#include "client.hpp"

#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t stop = 0;
void handle_signal(int) {
    stop = 1;
}

int main (void) {
    signal(SIGINT, handle_signal);
    Client client;
    
    while(!stop) {
        client.call();
        client.get_data();
        client.print_data();
        
        sleep(2);
    }
    
    return 0;
}
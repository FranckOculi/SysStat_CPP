#include "system.hpp"
#include "common.hpp"

#include <iostream>
#include <thread>
#include <chrono>

#define RED   "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
#define INTERVAL 2s

using namespace std::chrono_literals;

int main(void) {
    std::cout << "init stat" << std::endl;

    System::system_stats prev = {};
    System::system_stats current = {};

    System sys;
    int output = 0;

    if ((output = sys.system_infos(&prev)) != 0) {
        return output;
    }

    std::this_thread::sleep_for(INTERVAL); 

    if ((output = sys.system_infos(&current)) != 0) {
        return output;
    }

    Common::print_cpu(Common::calcul_cpu_active(&current, &prev));
    Common::print_mem(Common::calcul_mem_active(&current));
    Common::print_uptime(current.uptime_hours, current.uptime_minutes);

    return 0; 
}
#ifndef __COMMON__
#define __COMMON__

#include "system.hpp"
#include <cstdio>

#define RED   "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

#define INTERVAL 2s

class Common {
    public :
        static void print_cpu(int cpu);
        static void print_mem(int mem);
        static void print_uptime(int hours, int minutes);
        static int calcul_cpu_active(const System::system_stats *current, const System::system_stats *prev);
        static int calcul_mem_active(const System::system_stats *current);
        static void print_log(FILE *stream, const char *fmt, ...);
};

#endif
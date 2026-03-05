#ifndef __SYSTEM__
#define __SYSTEM__

#include <string>

class System {
    public :
        /* data from /proc/stat */
        struct cpu_stats {
            unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
            unsigned long long total, idle_time;
        };

        /* data from /proc/meminfo */
        struct mem_stats {
            unsigned long long mem_total, mem_available;
        };

        /* data from /proc/uptime */
        struct uptime_stats {
            int hours, minutes;
        };

        struct system_stats {
            cpu_stats cpu;
            mem_stats mem;
            uptime_stats uptime;
        };

        system_stats system_infos();

    private :
        cpu_stats cpu_info();
        mem_stats mem_info();
        uptime_stats uptime_info();
        template <typename F, typename T> T safe_call(F func, T stats);

        const std::string cpu_path = "/proc/stat";
        const std::string mem_path = "/proc/meminfo";
        const std::string uptime_path = "/proc/uptime";
};

#endif
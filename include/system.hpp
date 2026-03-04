#ifndef __SYSTEM__
#define __SYSTEM__

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

        struct system_stats {
            cpu_stats cpu;
            mem_stats mem;
            int uptime_hours; /* data from /proc/uptime */
            int uptime_minutes; /* data from /proc/uptime */
        };

        int system_infos(system_stats* system_stats);

    private :
        int cpu_info(cpu_stats* cpu);
        int mem_info(mem_stats* mem);
        int uptime_info(int *hours, int *minutes);
};

#endif
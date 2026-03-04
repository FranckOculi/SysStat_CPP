#include "system.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

int System::cpu_info(cpu_stats* cpu) {
    if (cpu == nullptr) {
        std::cerr << "cpu_info : NULL pointer provided\n";;
        return 1;
    }

    const std::string cpu_path = "/proc/stat";
    std::ifstream cpu_file(cpu_path);
    
    if (!cpu_file.is_open()) {
        throw std::runtime_error("cpu_info : Can't open " + cpu_path);
    }

    std::string line;
    if (!std::getline(cpu_file, line)) {
        throw std::runtime_error("cpu_info : Failed to read first line");
    }
    
    std::istringstream s(line);
    std::string label;

    if (
        !(s >> label >> cpu->user >> cpu->nice >> cpu->system >> cpu->idle >> cpu->iowait
            >> cpu->irq >> cpu->softirq >> cpu->steal >> cpu->guest >> cpu->guest_nice)
    ) {
        throw std::runtime_error("cpu_info : unexpected format");
    }

    cpu->total = cpu->user + cpu->nice + cpu->system + cpu->idle + cpu->iowait +
    cpu->irq + cpu->softirq + cpu->steal;
    cpu->idle_time = cpu->idle + cpu->iowait;

    return 0;
}

int System::mem_info(mem_stats* mem) {
    if (mem == nullptr) {
        std::cerr << "mem_info : NULL pointer provided\n";;
        return -1;
    }

    std::string mem_path = "/proc/meminfo";
    std::ifstream mem_file(mem_path);

    if (!mem_file.is_open()) {
        throw std::runtime_error("mem_info : Can't open : " + mem_path);
    }

    std::string line;
    std::string label;
    unsigned long long value = 0;

    mem->mem_total = 0;
    mem->mem_available = 0;


    while(std::getline(mem_file, line)) {
        std::istringstream s(line);
        if (std::getline(s, label, ':')) {
            s >> value;
            if (label == "MemTotal") mem->mem_total = value;
            else if (label == "MemAvailable") mem->mem_available = value;
        }
    }

    if (mem->mem_total == 0 || mem->mem_available == 0) {
        throw std::runtime_error("mem_info : failed to read memTotal or memAvailable\n");
    } 

    return 0;
}

int System::uptime_info(int* hours, int* minutes) {
    if (hours == nullptr || minutes == nullptr) {
        std::cerr << "uptime_info : NULL pointer provided\n";
        return -1;
    }

    std::string uptime_path = "/proc/uptime";
    std::ifstream uptime_file(uptime_path);
   
    if (!uptime_file.is_open()) {
        throw std::runtime_error("uptime_info : can't open : " + uptime_path);
    }

    double uptime, idle;

    std::string line;
    std::getline(uptime_file, line);
    std::istringstream s(line);
    if (!(s >> uptime >> idle)) {
        throw std::runtime_error("uptime_info : failed to read data");
    }
    
    *hours = (int)(uptime / 3600);
    *minutes = (int)(uptime / 60) % 60;

    return 0;
}

int System::system_infos(system_stats* system_stats) {
    if (system_stats == nullptr) {
        std::cerr << "system_infos : NULL pointer provided\n";
        return -1;
    }

    int output = 0;
    if ((output = cpu_info(&system_stats->cpu)) != 0) return output;
    if ((output = mem_info(&system_stats->mem)) != 0) return output;
    if ((output = uptime_info(&system_stats->uptime_hours, &system_stats->uptime_minutes)) != 0) return output;

    return output;
}



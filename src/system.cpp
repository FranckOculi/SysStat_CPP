#include "system.hpp"
#include "common.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

System::cpu_stats System::cpu_info() {
    cpu_stats cpu = {};
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
        !(s >> label >> cpu.user >> cpu.nice >> cpu.system >> cpu.idle >> cpu.iowait
            >> cpu.irq >> cpu.softirq >> cpu.steal >> cpu.guest >> cpu.guest_nice)
    ) {
        throw std::runtime_error("cpu_info : unexpected format");
    }

    cpu.total = cpu.user + cpu.nice + cpu.system + cpu.idle + cpu.iowait +
    cpu.irq + cpu.softirq + cpu.steal;
    cpu.idle_time = cpu.idle + cpu.iowait;

    return cpu;
}

System::mem_stats System::mem_info() {
    mem_stats mem = {};
    std::ifstream mem_file(mem_path);

    if (!mem_file.is_open()) {
        throw std::runtime_error("mem_info : Can't open : " + mem_path);
    }

    std::string line;
    std::string label;
    unsigned long long value = 0;

    mem.mem_total = 0;
    mem.mem_available = 0;


    while(std::getline(mem_file, line)) {
        std::istringstream s(line);
        if (std::getline(s, label, ':')) {
            s >> value;
            if (label == "MemTotal") mem.mem_total = value;
            else if (label == "MemAvailable") mem.mem_available = value;
        }
    }

    if (mem.mem_total == 0 || mem.mem_available == 0) {
        throw std::runtime_error("mem_info : failed to read memTotal or memAvailable\n");
    } 

    return mem;
}

System::uptime_stats System::uptime_info() {
    uptime_stats uptime = {};
    std::ifstream uptime_file(uptime_path);
   
    if (!uptime_file.is_open()) {
        throw std::runtime_error("uptime_info : can't open : " + uptime_path);
    }

    double uptime_value, idle_value;

    std::string line;
    std::getline(uptime_file, line);
    std::istringstream s(line);
    if (!(s >> uptime_value >> idle_value)) {
        throw std::runtime_error("uptime_info : failed to read data");
    }
    
    uptime.hours = (int)(uptime_value / 3600);
    uptime.minutes = (int)(uptime_value / 60) % 60;

    return uptime;
}

template <typename F, typename T> T System::safe_call(F func, T stats) {
    try {
        return func();
    } catch (...) {
        return stats;
    }
}

System::system_stats System::system_infos() {
    system_stats stats = {};
    stats.cpu = safe_call([this]{ return cpu_info(); }, stats.cpu);
    stats.mem = safe_call([this]{ return mem_info(); }, stats.mem);
    stats.uptime = safe_call([this]{ return uptime_info(); }, stats.uptime);

    return stats;
}



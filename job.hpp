#pragma once

#include <iostream>
#include <vector>

#include "enums.hpp"

using namespace std;

struct Process
{
    int cycles;
    ProcessType type;

    Process(int cycles, string type)
        : cycles(cycles)
    {
        this->type = type == "I/O" ? ProcessType::IO : ProcessType::CPU;
    };

    string str_type()
    {
        switch (type)
        {
        case ProcessType::IO:
            return "I/O";

        case ProcessType::CPU:
            return "CPU";

        default:
            return "";
        }
    }
    void run() { --cycles; };
};
struct Job
{
    int number;
    int arrivalTime;
    int completeTime;
    int totalCPUburst = 0;
    int totalIOburst = 0;
    vector<Process> list;

    int turnaround_time()
    {
        return completeTime - arrivalTime;
    }

    int waiting_time()
    {
        return turnaround_time() - totalCPUburst - totalIOburst;
    }

    void calculate_burst()
    {
        for (auto i : list)
        {
            switch (i.type)
            {
            case ProcessType::IO:
                totalIOburst += i.cycles;
                break;
            case ProcessType::CPU:
                totalCPUburst += i.cycles;
                break;
            }
        }
    }

    int count()
    {
        return list.size();
    }

    void add_process(Process process)
    {
        list.push_back(process);
    }

    void run()
    {
        if (count() == 0)
            return;

        list.at(0).run();
        if (list.at(0).cycles <= 0)
            list.erase(list.begin());
    }

    Process &current_process()
    {
        return *(list.begin());
    }

    void print()
    {
        cout << "Arrival Time : " << arrivalTime << endl;
        for (int i = 0; i < list.size(); i++)
        {
            cout << i + 1 << ". Type : " << list.at(i).str_type() << " , Cycles : " << list.at(i).cycles << endl;
        }
    }
};
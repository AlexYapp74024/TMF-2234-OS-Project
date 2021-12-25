#include <iostream>
#include <vector>
#include <array>

#include "job.hpp"
#include "enums.hpp"

using namespace std;

void round_robin(vector<Job> jobList)
{
    const int TIME_QUANTUM = 10;
    int jobCount = jobList.size();

    vector<Job> ready;
    vector<Job> waiting;
    vector<Job> running;
    vector<Job> terminated;

    for (int cycle = 0; terminated.size() < jobCount; cycle++)
    {
        // Identify new jobs
        // add to ready
        for (auto it = jobList.begin(); it < jobList.end(); it++)
        {
            if (cycle >= it->arrivalTime)
            {
                cout << cycle << " : "
                     << " Job " << it->number << " has arrived " << endl;
                ready.push_back(*it);
                jobList.erase(it);
            }
        }

        // In Waiting Jobs
        // Run I/O process
        // If current process = CPU , move to ready
        for (auto it = waiting.begin(); it < waiting.end(); it++)
        {
            Process process = it->current_process();
            if (process.type == ProcessType::IO)
            {
                it->run();
            }
            else if (process.type == ProcessType::CPU)
            {
                cout << cycle << " : "
                     << " Job " << it->number << " received I/O " << endl;
                ready.push_back(*it);
                waiting.erase(it);
            }
        }

        // In Ready jobs
        // For jobs waiting for I/O - Put to waiting
        // For jobs waiting for CPU - Put to running (if available)
        for (auto it = ready.begin(); it < ready.end(); it++)
        {
            Process process = it->current_process();
            if (process.type == ProcessType::IO)
            {
                cout << cycle << " : "
                     << " Job " << it->number << " wait for I/O " << endl;
                waiting.push_back(*it);
                ready.erase(it);
            }
            else if (process.type == ProcessType::CPU && running.empty())
            {
                cout << cycle << " : "
                     << " Job " << it->number << " is added to Running" << endl;
                running.push_back(*it);
                ready.erase(it);
            }
        }

        // In Running jobs
        // Run Process
        // Need I/O ? - Put to waiting
        // Used for 10 process? - Put to ready
        for (auto it = running.begin(); it < running.end(); it++)
        {
            Process process = it->current_process();
            static int cyclesUsed = 0;

            if (process.type == ProcessType::CPU)
            {
                it->run();
            }

            if (process.type == ProcessType::IO)
            {
                cout << cycle << " : "
                     << " Job " << it->number << " wait for I/O " << endl;
                waiting.push_back(*it);
                running.erase(it);
            }

            if (!running.empty())
                cyclesUsed++;
            else
                cyclesUsed = 0;

            if (cyclesUsed >= TIME_QUANTUM)
            {
                cout << cycle << " : "
                     << " Job " << it->number << " preempted (time quantum used up) " << endl;
                ready.push_back(*it);
                running.erase(it);
                cyclesUsed = 0;
            }
        }
        // Terminate empty jobs

        // lambda function that terminates jobs in given vectors
        auto terminate_job = [&](vector<Job> &vec)
        {
            for (auto it = vec.begin(); it < vec.end(); it++)
            {
                if (it->list.empty())
                {
                    cout << cycle << " : "
                         << " Job " << it->number << " terminated" << endl;
                    terminated.push_back(*it);
                    vec.erase(it);
                }
            }
        };

        // terminate job in these vectors
        terminate_job(ready);
        terminate_job(waiting);
        terminate_job(running);
    }
}
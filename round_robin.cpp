#include <iostream>
#include <vector>
#include <algorithm>

#include "job.hpp"
#include "enums.hpp"

using namespace std;

vector<Job> round_robin(vector<Job> jobList, int &out_interupts, int &out_totalCycles, int timeQuantum = 10)
{
    const bool LOG = false;
    int jobCount = jobList.size();

    vector<Job> ready;
    vector<Job> waiting;
    vector<Job> running;
    vector<Job> terminated;
    int cycle;

    // Job Processing
    for (cycle = 0; terminated.size() < jobCount; cycle++)
    {
        // Identify new jobs
        // add to ready
        for (auto it = jobList.begin(); it < jobList.end(); it++)
        {
            if (cycle >= it->arrivalTime)
            {
                if (LOG)
                    cout << cycle << " : Job " << it->number << " has arrived " << endl;
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
                if (LOG)
                    cout << cycle << " : Job " << it->number << " received I/O " << endl;
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
                if (LOG)
                    cout << cycle << " : Job " << it->number << " wait for I/O " << endl;
                waiting.push_back(*it);
                ready.erase(it);
            }
            else if (process.type == ProcessType::CPU && running.empty())
            {
                if (LOG)
                    cout << cycle << " : Job " << it->number << " is added to Running" << endl;
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
                if (cyclesUsed >= timeQuantum)
                {
                    if (LOG)
                        cout << cycle << " : Job " << it->number << " preempted (time quantum used up) " << endl;
                    ++out_interupts;
                    ready.push_back(*it);
                    running.erase(it);
                }
                else
                {
                    it->run();
                    cyclesUsed++;
                }
            }

            if (process.type == ProcessType::IO)
            {
                if (LOG)
                    cout << cycle << " : Job " << it->number << " wait for I/O " << endl;
                waiting.push_back(*it);
                running.erase(it);
            }

            if (running.empty())
                cyclesUsed = 0;
        }
        // Terminate empty jobs

        // Basics of lambda expressions cover by The Cherno, 2018
        // Details on lambda expressions are available in cppreference.com, 2021

        // lambda function that terminates jobs in given vectors
        auto terminate_job = [&](vector<Job> &vec)
        {
            for (auto it = vec.begin(); it < vec.end(); it++)
            {
                if (it->list.empty())
                {
                    if (LOG)
                        cout << cycle << " : Job " << it->number << " terminated" << endl;
                    it->completeTime = cycle;
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

    out_totalCycles = cycle;

    // Sorting function demonstrated by Bo Qian, 2013
    sort(terminated.begin(), terminated.end(), [](Job x, Job y)
         { return x.number < y.number; });
    return terminated;
}

// References
/*
    Bo Qian. (2013, Jan 16). STL Algorithms #3: Sorting [Video]. Youtube.
        https://www.youtube.com/watch?v=TZv5qHU2AMQ

    cppreference.com. (2021, Dec 6). Lambda expressions (since C++11).
        https://en.cppreference.com/w/cpp/language/lambda

    The Cherno. (2018, Feb 11). Lambdas in C++ [Video]. Youtube.
        https://www.youtube.com/watch?v=mWgmBBz0y8c
*/
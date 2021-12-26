#include <iostream>
#include <vector>

#include "csv_parsing.cpp"
#include "round_robin.cpp"
#include "job.hpp"
#include "enums.hpp"

using namespace std;

struct Results
{
    int interupts = 0;
    int timeQuantum = 10;
    int totalCycles = 0;
    vector<Job> jobList;

    Results(int time) : timeQuantum(time) {}

    double average_TAT()
    {
        double total = 0.0;
        for (Job job : jobList)
            total += job.turnaround_time();
        return (total / jobList.size());
    }
    double average_WaitingTime()
    {
        double total = 0.0;
        for (Job job : jobList)
            total += job.waiting_time();
        return (total / jobList.size());
    }
};

int main()
{
    // const string FILENAME = "Job List 11.csv";
    const string FILENAME = "Job List 11 3.csv";

    vector<Job> jobList = parseCSV(FILENAME);

    // Round Robin for each time Quantum
    vector<Results> results;
    for (int timeQuantum = 5; timeQuantum <= 200; timeQuantum += 5)
    {
        Results temp(timeQuantum);
        temp.jobList = round_robin(jobList, temp.interupts, temp.totalCycles, temp.timeQuantum);
        results.push_back(temp);
    }

    // Tabulate results
    {
        const char *TABLE_HEADER_FORMAT = "%8.8s | %6.6s | %15.15s | %12.12s | %9.9s \n";
        const char *TABLE_CONTENT_FORMAT = "%8.2i | %6i | %15.2f | %12.2f | %9i \n";
        const char *DASHES = "------------------------------";

        cout << endl;
        printf(TABLE_HEADER_FORMAT, "Time", "Total", "Average", "Average", "Number of");
        printf(TABLE_HEADER_FORMAT, "Quantum", "Cycles", "Turnaround Time", "Waiting Time", "Interupts");
        printf(TABLE_HEADER_FORMAT, DASHES, DASHES, DASHES, DASHES, DASHES);
        for (auto it = results.begin(); it < results.end(); it++)
        {
            printf(TABLE_CONTENT_FORMAT,
                   it->timeQuantum,
                   it->totalCycles,
                   it->average_TAT(),
                   it->average_WaitingTime(),
                   it->interupts);
        }
    }

    // Rank results
    {
        cout << endl;
        vector<Results>::iterator it;
        it = min_element(results.begin(), results.end(),
                         [](Results x, Results y)
                         { return x.totalCycles < y.totalCycles; });
        cout << "Time quantum " << it->timeQuantum << " has the least Total Cycles (" << it->totalCycles << ")" << endl;

        it = min_element(results.begin(), results.end(),
                         [](Results x, Results y)
                         { return x.average_TAT() < y.average_TAT(); });
        cout << "Time quantum " << it->timeQuantum << " has the least Turnaround Time (" << it->average_TAT() << ")" << endl;

        it = min_element(results.begin(), results.end(),
                         [](Results x, Results y)
                         { return x.average_WaitingTime() < y.average_WaitingTime(); });
        cout << "Time quantum " << it->timeQuantum << " has the least Waiting Time (" << it->average_WaitingTime() << ")" << endl;

        it = min_element(results.begin(), results.end(),
                         [](Results x, Results y)
                         { return x.interupts < y.interupts; });
        cout << "Time quantum " << it->timeQuantum << " has the least Interupts Incurred (" << it->interupts << ")" << endl;
    }

    cout << "\nOperation Successful!!\n"
         << endl;

    return 0;
}

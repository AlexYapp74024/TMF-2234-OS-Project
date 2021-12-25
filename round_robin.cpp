#include <iostream>
#include <vector>
#include <array>

#include "enums.hpp"

using namespace std;

void round_robin(vector<Job> jobList)
{
    const int TIME_QUANTUM = 10;

    vector<Job> ready;
    vector<Job> waiting;
    array<Job, 1> running;
    vector<Job> terminate;

    for (int cycle = 0; !jobList.empty() && cycle <= 10000; cycle++)
    {
        cout << cycle << " : ";

        cout << endl;
    };
}
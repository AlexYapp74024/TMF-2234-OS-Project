#include <iostream>
#include <vector>

#include "csv_parsing.cpp"
#include "round_robin.cpp"
#include "job.cpp"
#include "enums.hpp"

using namespace std;

int main()
{
    const string FILENAME = "Job List 11.csv";

    vector<Job> jobList;

    parseCSV(FILENAME, jobList);
    round_robin(jobList);

    cout << "\nOperation Successful!!\n"
         << endl;

    return 0;
}

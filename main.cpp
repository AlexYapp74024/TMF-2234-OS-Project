#include <iostream>
#include <vector>

#include "csv parsing.cpp"
#include "round robin.cpp"

using namespace std;

pair<vector<vector<string>>, vector<vector<int>>> getTestData(vector<int> &out_arrivalTime)
{
    // I/O (31)
    vector<vector<string>> processType;
    vector<vector<int>> cycles;

    processType = {{"I/O", "CPU", "CPU"},
                   {"CPU", "CPU", "I/O"},
                   {"CPU", "CPU", "CPU"}};

    cycles = {{30, 21, 13},
              {17, 5, 21},
              {9, 2, 11}};

    out_arrivalTime = {1, 2, 36};

    return make_pair(processType, cycles);
}

int main()
{
    const string FILENAME = "Job List 11.csv";

    pair<vector<vector<string>>, vector<vector<int>>> processes;
    vector<int> arrivalTime;

    // processes = getTestData(arrivalTime);

    parseCSV(FILENAME, arrivalTime);

    // for (auto i : arrivalTime)
    //     cout << i << " ";

    cout << "\nOperation Successful!!\n"
         << endl;

    return 0;
}

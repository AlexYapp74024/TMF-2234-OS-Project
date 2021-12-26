#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>

#include "job.hpp"
#include "enums.hpp"

using namespace std;

vector<Job> parseCSV(const string filename)
{
    vector<vector<string>> cells;
    vector<Job> jobs;

    // This section Extracts Data from each cells
    {
        // Opening CSV file from Yunus Kulyyev, 2018
        ifstream file;
        file.open(filename);

        while (file.good())
        {
            vector<string> rows;

            // extract each row
            string line;
            getline(file, line);
            rows.push_back(line);
            // extract cells in each row

            stringstream ss;
            for (int row = 0; row < rows.size(); row++)
            {
                vector<string> rowCells;

                ss << rows.at(row);
                while (ss.good())
                {
                    string cell;
                    getline(ss, cell, ',');
                    rowCells.push_back(cell);
                }
                cells.push_back(rowCells);
            }
        }
        file.close();
    }

    // This section Identifies and extracts each Process and Arrival Times
    {
        // Arrival times and processe are contained at row 3, 8, 13... (counting starts from 0)
        const int FIRST_COLUMN_NUMBER = 3;
        const int GAP_BETWEEN_PROCESSES = 5;

        // rowCount - 1 because the last row is empty
        int rowCount = cells.size() - 1;
        int colCount = cells.at(0).size();

        for (int col = FIRST_COLUMN_NUMBER; col < colCount; col += GAP_BETWEEN_PROCESSES)
        {
            // Usage of regex and subgroups are guided by posts made by
            // Dean, 2016 and Bo Qian, 2013
            Job job_current;

            smatch match;
            regex_search(cells.at(0).at(col), match, regex("[0-9]{1,}$"));
            job_current.arrivalTime = stoi(match.str());

            // for other rows, extract process types and cycle count
            for (int row = 1; row < rowCount; row++)
            {
                smatch match;
                // zeroth subgroup matches whole string
                // first subgroup matches CPU or I/O
                // second subgroup matches number in bracket
                regex_search(cells.at(row).at(col), match, regex("(CPU|I/O).+\\((\\d{1,})\\)$"));
                job_current.add_process(Process(stoi(match[2].str()), match[1].str()));
            }
            job_current.number = jobs.size() + 1;
            job_current.calculate_burst();
            jobs.push_back(job_current);
        }
    }

    return jobs;
}

// References
/*
    Bo Qian. (2013, Mar 9). C++ 11 Library: Regular Expression 2 -- Submatch [Video]. Youtube.
        https://www.youtube.com/watch?v=nkjUpUu3dFk&list=PL5jc9xFGsL8FWtnZBeTqZBbniyw0uHyaH&index=13

    Dean B. (2016, Oct 16). I rewrote ip_reg to use sub-patterns and print_results to use iterators
        [Comment on the online forum post "c++ regex: how to use sub-matches".]. Stack Overflow.
        https://stackoverflow.com/questions/39946618/c-regex-how-to-use-sub-matches

    Yunus Kulyyev. (2018, Mar 6). C++ Tutorials- CSV input file [Video]. Youtube.
        https://www.youtube.com/watch?v=da8prmVYul8
*/
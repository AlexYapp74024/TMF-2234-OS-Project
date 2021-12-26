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
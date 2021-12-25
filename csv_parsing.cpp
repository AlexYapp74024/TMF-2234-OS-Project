#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>

#include "job.cpp"
#include "enums.hpp"

using namespace std;

void parseCSV(const string filename,
              vector<Job> &out_jobs)
{
    vector<vector<string>> cells;

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

        // for (int row = 0; row < rowCount; row++)
        // {
        //     for (int col = 0; col < colCount; col++)
        //     {
        //         cout << cells.at(row).at(col) << " ";
        //     }
        //     cout << endl;
        // }

        for (int col = FIRST_COLUMN_NUMBER; col < colCount; col += GAP_BETWEEN_PROCESSES)
        {
            Job job_current;

            // for row 0, extract arrival time
            // cout << cells.at(0).at(col) << endl;s

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
                // cout << cells.at(row).at(col) << " : ";
                regex_search(cells.at(row).at(col), match, regex("(CPU|I/O).+\\((\\d{1,})\\)$"));
                // for (auto pattern : match)
                //     cout << pattern << " ";
                // cout << endl;
                job_current.add_process(Process(stoi(match[2].str()), match[1].str()));
            }
            out_jobs.push_back(job_current);
        }
    }
}
/***************************************************************************************************
 * CSV Access Demo
 *
 * This is an example of how to use the CSV Wrapper
 *
 * TODO: Better documentation, more complete testing
 *
 **************************************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "csv_wrapper.h"

using namespace std;

/***************************************************************************************************
 * Write CSV
 *
 * Demonstration of writing five lines to a CSV file, note that the lines can vary in length,
 * although this would make extracting columns difficult.
 *
 * Note that this will overwrite the CSV file every time. If you need to append, let Matthew know
 * and it'll take like ~5 minutes to implement.
 *
 **************************************************************************************************/
void writeCSV()
{
    vector<vector<string>> rows;

    vector<string> row;
    for(int i = 0; i < 5; i++)
    {
        row.clear();

        row.push_back(to_string(i));

        switch(i)
        {
            case 0:
                row.push_back("Mercy");
                row.push_back("Damage-Boosting");
                row.push_back("Extra!");

                break;
            case 1:
                row.push_back("Mercy");
                row.push_back("None");
                break;
            case 2:
                row.push_back("Lucio");
                //row.push_back("Booping");
                break;
            case 3:
                row.push_back("Lucio");
                row.push_back("None");
                break;
            case 4:
                row.push_back("Soldier");
                row.push_back("Firing");
                break;
        }

        rows.push_back(row);
    }

    csv_wrapper::saveToCSV("test.csv", rows);
}

/***************************************************************************************************
 * Open CSV
 *
 * Demonstration of opening a CSV File. You must pass a vector<vector<string>> to be filled by
 * the method. This allows for us to use stack memory, rather than having the CSV function
 * allocate heap memory which you'd need to delete manually.
 *
 **************************************************************************************************/
void openCSV()
{
    vector<vector<string>> openedValues;

    csv_wrapper::readFromCSV("test.csv", openedValues);

    for(int i = 0; i < openedValues.size(); i++)
    {
        for(int j = 0; j < openedValues[i].size(); j++)
        {
            cout << openedValues[i][j];

            if(j < openedValues[i].size() - 1)
            {
                cout << " : ";
            }
        }

        cout << endl;
    }
}


/***************************************************************************************************
 * Main
 *
 * Writes the CSV file, then reads from it.
 *
 **************************************************************************************************/
int main()
{
    writeCSV();

    openCSV();

    return 0;
}


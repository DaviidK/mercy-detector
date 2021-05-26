/***************************************************************************************************
 * CSV Wrapper Interface
 *
 * This class allows for Comma Seperated Value (CSV) files to be saved and opened. It represents
 * CSV files as a two dimensional vector array. Each row is a vector of strings of arbitrary length.
 * Rows are placed into a vector of vector<string>.
 *
 * Note: Whitespace before and after an individual value will be trimmed! For example, " Lucio "
 * will be saved and opened as "Lucio".
 *
 **************************************************************************************************/

#ifndef MERCY_DETECTOR_CSV_WRAPPER_H
#define MERCY_DETECTOR_CSV_WRAPPER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

static const string DELIMITER = ", ";

class csv_wrapper {

public:

    /***********************************************************************************************
     * Save To CSV
     *
     * Creates or overwrites a csv file at the provided file path. The CSV file is populated with
     * the 2D string data vector.
     *
     * TODO: Invalid file handling, append options
    ***********************************************************************************************/
    static void saveToCSV(const string& filename, vector<vector<string>> data);

    /***********************************************************************************************
     * Read From CSV
     *
     * Opens a csv file at the provided file path. Populates the provided 2D string vector with the
     * contents of the csv file.
     *
     * TODO: Invalid file handling
    ***********************************************************************************************/
    static void readFromCSV(const string& filename, vector<vector<string>>& data);

private:

    /***********************************************************************************************
     * Trim
     *
     * Trims whitespace from a provided string
     *
     * Source: https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
    ***********************************************************************************************/
    static string trim(const string& str, const string& whitespace = " \t");

    /***********************************************************************************************
     * Reduce
     *
     * Removes whitespace from the beginning and end of a provided string
     *
     * Source: https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
    ***********************************************************************************************/
    static string reduce(const string& str, const string& fill = " ", const string& whitespace = " \t");
};


#endif //MERCY_DETECTOR_CSV_WRAPPER_H

/***************************************************************************************************
 * CSV Wrapper Implementation
 *
 * This class allows for Comma Seperated Value (CSV) files to be saved and opened. It represents
 * CSV files as a two dimensional vector array. Each row is a vector of strings of arbitrary length.
 * Rows are placed into a vector of vector<string>.
 *
 **************************************************************************************************/

#include "csv_wrapper.h"

/***************************************************************************************************
 * Save To CSV
 *
 * Opens the provided filename. Current open mode is hardcoded to truncate, which will overwrite
 * existing files.
 *
 * Loops through the provided 2D string vector. For each element in the row, the string is
 * written followed by the delimiter. If the element is the last element in the row, a newline
 * character is added.
 *
 **************************************************************************************************/
void csv_wrapper::saveToCSV(const string& filename, vector<vector<string>> data)
{
    fstream fileOut;

    fileOut.open(filename, ios::trunc | ios::out);

    for(int i = 0; i < data.size(); i++)
    {
        for(int j = 0; j < data[i].size(); j++)
        {
            fileOut << data[i][j];

            if(j < data[i].size() - 1)
            {
                fileOut << DELIMITER;
            }
            else
            {
                fileOut << "\n";
            }
        }
    }

    fileOut.close();
}

/***************************************************************************************************
 * Read From CSV
 *
 * Opens a file from the provided filename.
 *
 * Reads through each line in the provided file. Uses commas as a delimiter, then removes
 * whitespace using the reduce() method. This means that all values within the data array
 * will have no whitespace before or after the string.
 *
 **************************************************************************************************/
void csv_wrapper::readFromCSV(const string& filename, vector<vector<string>>& data)
{
    fstream fileIn;

    fileIn.open(filename);

    vector<string> row;
    string line, word;

    while(getline(fileIn, line))
    {
        row.clear();
        stringstream s(line);

        while(getline(s, word, ','))
        {
            word = reduce(word);

            row.push_back(word);
        }

        data.push_back(row);
    }
}

/***************************************************************************************************
 * Reduce
 *
 * Removes the whitespace from before and after a provided string.
 *
 * Source: https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
 **************************************************************************************************/
string csv_wrapper::reduce(const string &str, const string &fill, const string &whitespace)
{
    // trim first
    auto result = trim(str, whitespace);

    // replace sub ranges
    auto beginSpace = result.find_first_of(whitespace);
    while (beginSpace != std::string::npos)
    {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        result.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
    }

    return result;
}

/***************************************************************************************************
 * Trim
 *
 * Removes whitespace from a string
 *
 * Source: https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
 **************************************************************************************************/
string csv_wrapper::trim(const string &str, const string &whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}
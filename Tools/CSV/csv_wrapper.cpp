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
 * Creates or overwrites a csv file at the provided file path. The CSV file is populated with
 * the 2D string data vector.
 *
 * TODO: Invalid file handling, append options
 * 
 * @param filename: The filename in which the resulting CSV will be saved
 * @param data: A 2D vector of strings which contains the data to write to a CSV
 * 
 * @post: A new CSV file will be created at the location specified by 'filename', containing the 
 *        data specified by 'data'
***************************************************************************************************/
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
 * Opens a csv file at the provided file path. Populates the provided 2D string vector with the
 * contents of the csv file.
 *
 * @param filename: The filename from which to read in data
 * @param data: A 2D vector of strings which will save the data read in from a CSV
 * 
 * @pre: A CSV file exists at the location
 * @post: A new CSV file will be created at the location specified by 'filename', containing the 
 *        data specified by 'data'
***************************************************************************************************/
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
 * Trim
 *
 * Trims whitespace from a provided string
 * Source: https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
 * 
 * @param str: String which will be trimmed
 * @param whitespace: Whitespace to removed from the string
 * 
 * @post: The passed parameter string will have whitespace removed
****************************************************************************************************/
string csv_wrapper::trim(const string &str, const string &whitespace)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

/***************************************************************************************************
 * Reduce
 *
 * Removes whitespace from the beginning and end of a provided string, replacing it with a given
 * character.
 * Source: https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string
 * 
 * @param str: String which will be trimmed
 * @param fill: Character used to replace trimmed whitespace
 * @param whitespace: Whitespace to removed from the string
 * 
 * @post: The passed parameter string will have whitespace removed, and instead replaced with 
 *        the specified string
***************************************************************************************************/
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
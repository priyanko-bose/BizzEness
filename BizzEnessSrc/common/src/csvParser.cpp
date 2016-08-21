#include <iterator>
#include <iostream>
#include <sstream>
#include "common/include/csvParser.h"

using namespace std;
extern ofstream fout;

/* This is an overloaded >> operator to create a
 * CSV object from each line of the CSV file
 */
istream& operator>>(std::istream& str,CSVRow& data)
{
    data.readNextRow(str);
    return str;
}

/*
 * This function over loads the [] operator and
 * it returns the nth data of a vector
 */
string const& CSVRow::operator[](std::size_t index) const
{
    return m_data[index];
}

/*
 * Returns the total no of elements in a vector
 */
size_t CSVRow::size() const
{
    return m_data.size();
}

/*
 * This function reads the lines of a .csv file
 * one by one and inserts each text seperated by comma
 * as an element in a vector
 */
void CSVRow::readNextRow(std::istream& str)
{
    std::string line;
    //read the line
    std::getline(str,line);
    //convert to stream
    std::stringstream lineStream(line);
    std::string  cell;

    m_data.clear();
    //get each text separated by comma
    while(std::getline(lineStream,cell,','))
    {
        m_data.push_back(cell); // push into the vector
    }
}


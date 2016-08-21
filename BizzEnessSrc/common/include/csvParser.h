#ifndef BE_CSVPARSER_H
#define BE_CSVPARSER_H

#include <string>
#include <istream>
#include <vector>

using namespace std;

class CSVRow
{
    public:
        string const& operator[](size_t index) const;
        size_t size() const;
        void readNextRow(istream& str);

    private:
        vector<string> m_data;
};
istream& operator>>(std::istream& ,CSVRow& );
#endif

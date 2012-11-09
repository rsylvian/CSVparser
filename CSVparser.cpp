#include <fstream>
#include <sstream>
#include <iomanip>
#include "CSVparser.hpp"

CSVparser::CSVparser(const std::string &file, char sep)
    : _sep(sep)
{
    std::ifstream ifile(file);
    std::string line;

    if (ifile.is_open())
    {
        while (ifile.good())
        {
            getline(ifile, line);
            if (line != "")
                _originalFile.push_back(line);
        }
        ifile.close();
        parseHeader();
        parseContent();
    }
    else
        throw CSVError(std::string("Failed to open ").append(file));
}

CSVparser::~CSVparser(void)
{
   std::vector<CSVrow *>::iterator it;

   for (it = _content.begin(); it != _content.end(); it++)
        delete *it;
}

void CSVparser::parseHeader(void)
{
    std::stringstream ss(_originalFile[0]);
    std::string item;

    while (std::getline(ss, item, _sep))
        _header.push_back(item);
}

void CSVparser::parseContent(void)
{
   std::vector<const std::string>::iterator it;
   
   it = _originalFile.begin();
   it++; // skip header

   for (; it != _originalFile.end(); it++)
   {
       bool quoted = false;
       int tokenStart = 0;
       int i = 0;
       
       CSVrow *row = new CSVrow(_header);

       for (; i != it->length(); i++)
       {
            if (it->at(i) == '"')
                quoted = ((quoted) ? (false) : (true));
            else if (it->at(i) == ',' && !quoted)
            {
                row->push(it->substr(tokenStart, i - tokenStart));
                tokenStart = i + 1;
            }
       }

       //end
       row->push(it->substr(tokenStart, it->length() - tokenStart));
       _content.push_back(row);
   }
}

CSVrow &CSVparser::getRow(unsigned int rowPosition) const
{
    if (rowPosition >= 0 && rowPosition < _content.size())
        return *(_content[rowPosition]);
    throw CSVError("can't return this row (doesn't exist)");
}

CSVrow &CSVparser::operator[](unsigned int rowPosition) const
{
    return CSVparser::getRow(rowPosition);
}

unsigned int CSVparser::rowCount(void) const
{
    return _content.size();
}

unsigned int CSVparser::columnCount(void) const
{
    return _header.size();
}

std::vector<const std::string> CSVparser::getHeader(void) const
{
    return _header;
}

/*
** ROW
*/

CSVrow::CSVrow(const std::vector<const std::string> &header)
    : _header(header) {}

CSVrow::~CSVrow(void) {}

void CSVrow::push(const std::string &value)
{
    _values.push_back(value);
}

const std::string CSVrow::operator[](unsigned int valuePosition) const
{
     if (valuePosition >= 0 && valuePosition < _values.size())
         return _values[valuePosition];
     throw CSVError("can't return this value (doesn't exist)");
}

const std::string CSVrow::operator[](const std::string &valueName) const
{
    std::vector<const std::string>::const_iterator it;
    int pos = 0;

    for (it = _header.begin(); it != _header.end(); it++)
    {
        if (valueName == *it)
            return _values[pos];
        pos++;
    }
    
    throw CSVError("can't return this value (doesn't exist)");
}

std::ostream &operator<<(std::ostream &os, const CSVrow &row)
{
    for (int i = 0; i != row._values.size(); i++)
        os << row._values[i] << " | ";

    return os;
}

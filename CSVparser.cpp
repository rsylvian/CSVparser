#include "CSVparser.hpp"

#include <fstream>
#include <iomanip>
#include <sstream>

using std::fstream;
using std::ifstream;
using std::ios;
using std::istringstream;
using std::ofstream;
using std::ostream;
using std::string;
using std::stringstream;
using std::vector;

using std::getline;

namespace csv
{
    Parser::Parser(const string& data, const DataType& type, char sep)
        : _type(type), _sep(sep)
    {
        string line;
        if (type == eFILE)
        {
            _file = data;
            ifstream ifile(_file);
            if (ifile.is_open())
            {
                while (ifile.good())
                {
                    getline(ifile, line);
                    if (!line.empty())
                        _originalFile.push_back(line);
                }
                ifile.close();

                if (_originalFile.size() == 0)
                    throw Error(string("No Data in ").append(_file));

                parseHeader();
                parseContent();
            }
            else
                throw Error(string("Failed to open ").append(_file));
        }
        else
        {
            istringstream stream(data);
            while (getline(stream, line))
                if (!line.empty())
                    _originalFile.push_back(line);
            if (_originalFile.size() == 0)
                throw Error(string("No Data in pure content"));

            parseHeader();
            parseContent();
        }
    }

    Parser::~Parser()
    {
        for (auto it = _content.begin(); it != _content.end(); ++it)
            delete *it;
    }

    void Parser::parseHeader()
    {
        stringstream ss(_originalFile[0]);
        string item;

        while (getline(ss, item, _sep))
            _header.push_back(item);
    }

    void Parser::parseContent()
    {
        // skip header
        for (auto it = ++_originalFile.begin(); it != _originalFile.end(); ++it)
        {
            auto quoted = false;
            auto tokenStart = 0;

            auto* row = new Row(_header);

            for (auto i = 0; i != it->length(); ++i)
            {
                if (it->at(i) == '"')
                    quoted = ((quoted) ? (false) : (true));
                else if (it->at(i) == _sep && !quoted)
                {
                    row->push(it->substr(tokenStart, i - tokenStart));
                    tokenStart = i + 1;
                }
            }

            // end
            row->push(it->substr(tokenStart, it->length() - tokenStart));

            // if value(s) missing
            if (row->size() != _header.size())
                throw Error("corrupted data !");
            _content.push_back(row);
        }
    }

    Row& Parser::getRow(unsigned int rowPosition) const
    {
        if (rowPosition < _content.size())
            return *(_content[rowPosition]);
        throw Error("can't return this row (doesn't exist)");
    }

    Row& Parser::operator[](unsigned int rowPosition) const
    {
        return Parser::getRow(rowPosition);
    }

    unsigned int Parser::rowCount() const { return _content.size(); }

    unsigned int Parser::columnCount() const { return _header.size(); }

    vector<string> Parser::getHeader() const { return _header; }

    const string Parser::getHeaderElement(unsigned int pos) const
    {
        if (pos >= _header.size())
            throw Error("can't return this header (doesn't exist)");
        return _header[pos];
    }

    bool Parser::deleteRow(unsigned int pos)
    {
        if (pos < _content.size())
        {
            delete *(_content.begin() + pos);
            _content.erase(_content.begin() + pos);
            return true;
        }
        return false;
    }

    bool Parser::addRow(unsigned int pos, const vector<string>& r)
    {
        auto* row = new Row(_header);

        for (auto it = r.begin(); it != r.end(); ++it)
            row->push(*it);

        if (pos <= _content.size())
        {
            _content.insert(_content.begin() + pos, row);
            return true;
        }
        return false;
    }

    void Parser::sync() const
    {
        if (_type == DataType::eFILE)
        {
            ofstream f;
            f.open(_file, ios::out | ios::trunc);

            // header
            auto i = 0;
            for (auto it = _header.begin(); it != _header.end(); ++it)
            {
                f << *it;
                if (i < _header.size() - 1)
                    f << ",";
                else
                    f << '\n';
                ++i;
            }

            for (auto it = _content.begin(); it != _content.end(); ++it)
                f << **it << '\n';
            f.close();
        }
    }

    const string& Parser::getFileName() const { return _file; }

    /*
    ** ROW
    */

    Row::Row(const vector<string>& header) : _header(header) {}

    Row::~Row() {}

    unsigned int Row::size() const { return _values.size(); }

    void Row::push(const string& value) { _values.push_back(value); }

    bool Row::set(const string& key, const string& value)
    {
        auto pos = 0;

        for (auto it = _header.begin(); it != _header.end(); ++it)
        {
            if (key == *it)
            {
                _values[pos] = value;
                return true;
            }
            ++pos;
        }
        return false;
    }

    const string Row::operator[](unsigned int valuePosition) const
    {
        if (valuePosition < _values.size())
            return _values[valuePosition];
        throw Error("can't return this value (doesn't exist)");
    }

    const string Row::operator[](const string& key) const
    {
        auto pos = 0;

        for (auto it = _header.begin(); it != _header.end(); ++it)
        {
            if (key == *it)
                return _values[pos];
            ++pos;
        }

        throw Error("can't return this value (doesn't exist)");
    }

    ostream& operator<<(ostream& os, const Row& row)
    {
        for (auto i = 0; i != row._values.size(); ++i)
            os << row._values[i] << " | ";

        return os;
    }

    ofstream& operator<<(ofstream& os, const Row& row)
    {
        for (auto i = 0; i != row._values.size(); ++i)
        {
            os << row._values[i];
            if (i < row._values.size() - 1)
                os << ",";
        }
        return os;
    }
} // namespace csv
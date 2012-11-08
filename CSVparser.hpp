#ifndef CSVPARSER_HPP
# define CSVPARSER_HPP

# include <stdexcept>
# include <string>
# include <vector>
# include <list>
# include <iostream>

class CSVError : public std::runtime_error
{

  public:
    CSVError(const std::string &msg):
      std::runtime_error(std::string("CSVparser : ").append(msg))
    {
    }
};

class CSVrow
{
	public:
	    CSVrow(const std::vector<const std::string> &);
	    ~CSVrow(void);

	public:
        void push(const std::string &);
        unsigned int columnCount(void) const;

	private:
		const std::vector<const std::string> _header;
		std::vector<const std::string> _values;

    public:
        const std::string operator[](unsigned int) const;
        const std::string operator[](const std::string &valueName) const;
        friend std::ostream& operator<<(std::ostream& os, const CSVrow &row);
};

class CSVparser
{

public:
    CSVparser(const std::string &, char sep = ',');
    ~CSVparser(void);

public:
    CSVrow &getRow(unsigned int row) const;
    unsigned int rowCount(void) const;
    std::vector<const std::string> getHeader(void) const;

protected:
	void parseHeader(void);
	void parseContent(void);

private:
    const char _sep;
    std::vector<const std::string> _originalFile;
    std::vector<const std::string> _header;
    std::vector<CSVrow *> _content;

public:
    CSVrow &operator[](unsigned int row) const;
};

#endif // CSVPARSER_HPP

CSVparser
=========

CSVparser intends to be a simple C++ parser for the CSV file format.

What is a CSV file ?
--------------------

CSV is a common, relatively simple file format that is widely supported by consumer, business, and scientific applications. Among its most common uses is moving tabular data between programs that natively operate on incompatible (often proprietary and/or undocumented) formats. This works because so many programs support some variation of CSV at least as an alternative import/export format.

Compilation
-----------

```bash
g++ CSVparser.cpp -std=c++0x
```

Usage
-----

The following example shows how to make a simple usage of CSVparser.<br />
Assuming a common CSV file :

```
Year,Make,Model
1997,Ford,E350
2000,Mercury,Cougar
```

```c++
#include <iostream>
#include "CSVparser.hpp"

int main(int argc, char **argv)
{
	try
    {
        csv::Parser file = csv::Parser("files/readme.csv");

        std::cout << file[0][0] << std::endl; // display : 1997
        std::cout << file[0] << std::endl; // display : 1997 | Ford | E350

        std::cout << file[1]["Model"] << std::endl; // display : Cougar

        std::cout << file.rowCount() << std::endl; // display : 2
        std::cout << file.columnCount() << std::endl; // display : 3

        std::cout << file.getHeaderElement(2) << std::endl; // display : Model
    }
    catch (csv::Error &e)
    {
        std::cerr << e.what() << std::endl;
    }
  	return 0;
}
```
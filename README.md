CSVparser
=========

CSVparser intends to be a simple C++ parser for the CSV file format.

What is a CSV file ?
--------------------

CSV is a common, relatively simple file format that is widely supported by consumer, business, and scientific applications. Among its most common uses is moving tabular data between programs that natively operate on incompatible (often proprietary and/or undocumented) formats. This works because so many programs support some variation of CSV at least as an alternative import/export format.

Compilation
-----------

***Compile as Executable***
   
It requires the main() function to be defined in CSVparser.cpp (or an 
additional translation unit. See Usage in README for details).
```bash
$ g++ CSVparser.cpp -std=c++11
```

***Compile as Shared Library***

The build process is managed by CMake.
```
# from the root directory of the source files
$ mkdir build
$ cd build
$ cmake ..
# the following command can be replaced by make
$ cmake --build .
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
#include "CSVparser.hpp"

#include <iostream>

int main(int argc, char **argv)
{
	try
    {
        csv::Parser file = csv::Parser("files/readme.csv");

        std::cout << file[0][0] << '\n'; // display : 1997
        std::cout << file[0] << '\n'; // display : 1997 | Ford | E350

        std::cout << file[1]["Model"] << '\n'; // display : Cougar

        std::cout << file.rowCount() << '\n'; // display : 2
        std::cout << file.columnCount() << '\n'; // display : 3

        std::cout << file.getHeaderElement(2) << '\n'; // display : Model
    }
    catch (csv::Error &e)
    {
        std::cerr << e.what() << '\n';
    }
  	return 0;
}
```
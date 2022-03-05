project(csv)

set(CMAKE_CXX_STANDARD 11)

add_library(csv-parser
  CSVparser.cpp
  CSVparser.hpp
)
// expmain.cpp
//
// Do whatever you'd like here.  This is intended to allow you to experiment
// with the code in the "app" directory or with any additional libraries
// that are part of the project, outside of the context of the main
// application or Google Test.

#include <iostream>
#include <sstream>
#include <string>

int countWords(const std::string& line)
{
    int count;
    std::stringstream ss(line);
    std::string word;
    for (count=0; ss >> word; ++count) {
    }
    return count;
}

int main()
{
    int count = 0;
    std::string line;
    std::string word;
    while (true) {
        std::getline(std::cin, line);
        count = countWords(line);
        std::cout << "The line: " << line << " has " << count << " words." << std::endl;
    }



    return 0;
}


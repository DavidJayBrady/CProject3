#include <iostream>
#include <string>
#include "HashMap.hpp"
#include <sstream>

int countWords(const std::string& line)
{
    std::stringstream ss;
    int i;
    for (i=0; ss << line; ++i) {
    }
    return i;
}

int main()
{
    HashMap hashMap;
    while (true)
    {
        std::string firstWord;
        std::cin >> firstWord;

        if (firstWord == "QUIT") {
            break;
        }
        else if (firstWord == "GRAPH") {
            hashMap.printBuckets();
        }
        else if (firstWord == "CREATE") {
            std::string key, value;
            std::cin >> key >> value;
            hashMap.add(key, value);
        }
        else if (firstWord == "REMOVE") {
            std::string key;
            std::cin >> key;
            hashMap.remove(key);
        }

        // Clear the input buffer
        std::getline(std::cin, firstWord);

    }
    return 0;
}
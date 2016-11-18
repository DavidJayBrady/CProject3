#include <iostream>
#include <string>
#include "HashMap.hpp"
#include <sstream>

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
    // initialize necessary variables.
    // hashMap for doing the heavy lifting
    // word count ensuring not too few or many parameters
    // line for storing line of input
    // wordReader to help counting and giving word1, word2, and word3 their words
    // word1, word2, word3 store the info needed for commands
    // should never be more than 3 words according to project write up
    HashMap hashMap;
    int wordCount;
    std::string line;
    std::stringstream wordReader;
    std::string word1, word2, word3;

    // control whether debug features are on or off
    bool debugFlag = false;

    // used for detecting whether anything was added or removed
    int sizeBeforeChange, sizeAfterChange;

    while (true)
    {
        std::getline(std::cin, line);
        wordCount = countWords(line);

        //empty and refill with new line of input
        wordReader = std::stringstream(line);

        wordReader >> word1 >> word2 >> word3;

        if (word1 == "QUIT" && wordCount == 1) {
            std::cout << "GOODBYE" << std::endl;
            break;
        }
        else if (word1 == "GRAPH") {
            hashMap.printBuckets();
        }
        else if (word1 == "CREATE" && wordCount == 3) {
            sizeBeforeChange = hashMap.size();
            hashMap.add(word2, word3);
            sizeAfterChange = hashMap.size();
            if (sizeBeforeChange != sizeAfterChange) {
                std::cout << "CREATED" << std::endl;
            }
            else {
                std::cout << "EXISTS" << std::endl;
            }
        }
        else if (word1 == "LOGIN" && wordCount == 3) {
            if (hashMap.contains(word2) && hashMap.value(word2) == word3) {
                std::cout << "SUCCEEDED" << std::endl;
            }
            else {
                std::cout << "FAILED" << std::endl;
            }
        }
        else if (word1 == "REMOVE" && wordCount == 2) {
            sizeBeforeChange = hashMap.size();
            hashMap.remove(word2);
            sizeAfterChange = hashMap.size();
            if (sizeBeforeChange != sizeAfterChange) {
                std::cout << "REMOVED" << std::endl;
            }
            else {
                std::cout << "NONEXISTENT" << std::endl;
            }
        }
        else if (word1 == "CLEAR" && wordCount == 1) {
            hashMap.clearAndResetSize();
            std::cout << "CLEARED" << std::endl;
        }
        else if (word1 == "DEBUG" && word2 == "ON" && wordCount == 2) {
            if (debugFlag) {
                std::cout << "ON ALREADY" << std::endl;
            }
            else {
                debugFlag = true;
                std::cout << "ON NOW" << std::endl;
            }
        }
        else if (word1 == "DEBUG" && word2 == "OFF" && wordCount == 2) {
            if (debugFlag) {
                debugFlag = false;
                std::cout << "OFF NOW" << std::endl;
            }
            else {
                std::cout << "OFF ALREADY" << std::endl;
            }
        }
        else if (word1 == "LOGIN" && word2 == "COUNT" &&
                          wordCount == 2 && debugFlag) {
            std::cout << hashMap.size() << std::endl;
        }
        else if (word1 == "BUCKET" && word2 == "COUNT" &&
                 wordCount == 2 && debugFlag) {
            std::cout << hashMap.bucketCount() << std::endl;
        }
        else if (word1 == "LOAD" && word2 == "FACTOR" &&
                 wordCount == 2 && debugFlag) {
            std::cout << hashMap.loadFactor() << std::endl;
        }
        else if (word1 == "MAX" && word2 == "BUCKET" &&
                word3 == "SIZE" && wordCount == 3 && debugFlag) {
            std::cout << hashMap.loadFactor() << std::endl;
        }
        else {
            std::cout << "INVALID" << std::endl;
        }
    }
    return 0;
}
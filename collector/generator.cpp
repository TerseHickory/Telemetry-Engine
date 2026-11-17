#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>

std::vector<std::string> getfileData(const std::string& filePath, const std::vector<std::string_view> substring);
std::vector<std::string_view> parseData(std::string_view line, const char* delimiters);
std::vector<std::vector<std::string_view>> createDataList(const std::vector<std::string_view>& fileData, const char delimiters);

int main()
{

    /* Test Code
    const std::vector<std::string_view> x {"cpu"};
    std::vector<std::string> storedData;
    readFile("/proc/stat", storedData, x);

    std::cout << storedData[0] << '\n' << storedData[1] << '\n' << storedData[10] << std::endl;
    */

    return 0;
};

struct CPU 
{
    std::string name;
    std::string userTime;
    std::string niceTime;
    std::string systemTime;
    std::string idleTime;
    std::string iowaitTime;
    std::string irqTime;
    std::string softirqTime;
    std::string stealTime;
    std::string speed;
};

// Returns lines from chosen data file that include specified keyword(s)
std::vector<std::string> getfileData(const std::string& filePath, const std::vector<std::string_view>& substring)
{
    std::vector<std::string> fileData {};
    std::ifstream file (filePath.c_str());

    if (!file)
    {
        std::cerr << "Uh oh, file could not be opened!" << std::endl;
        exit(1);
    }

    while (file)
    {
        std::string strInput {};
        getline(file, strInput);
        for (const std::string_view word : substring) 
        {
            const unsigned long result {strInput.find(word)};

            if (result != std::string_view::npos)
            {
                fileData.push_back(strInput);
                continue;
            }
        }
    }

    return fileData;
};

// Parses string into vector by specified delimiters
std::vector<std::string_view> parseData(std::string_view line, const char* delimiters) 
{
    std::vector<std::string_view> tokens {};
    size_t start {0};
    size_t end {line.find(*delimiters)};

    while (end != std::string_view::npos)
    {
        tokens.push_back(line.substr(start, end - start));
        start = end + 1;
        end = line.find(*delimiters, start);
    }
    tokens.push_back(line.substr(start));

    return tokens;
};

// Creates a multidimensional vector that separates data by values and by line
std::vector<std::vector<std::string_view>> createDataList(const std::vector<std::string_view>& fileData, const char delimiters)
{
    std::vector<std::vector<std::string_view>> dataList {};

    for (std::string_view line : fileData)
    {
        dataList.push_back(parseData(line, &delimiters));
    }

    return dataList;
};
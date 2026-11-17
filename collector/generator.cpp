#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void readFile(const std::string& filePath, std::vector<std::string>& sampleData, const std::vector<std::string_view> substring);

int main() {

    /* Test Code
    const std::vector<std::string_view> x {"cpu"};
    std::vector<std::string> storedData;
    readFile("/proc/stat", storedData, x);

    std::cout << storedData[0] << '\n' << storedData[1] << '\n' << storedData[10] << std::endl;
    */

    return 0;
};

struct CPU {
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

std::vector<std::string> cpuData;


// Read data file and copy lines with chosen substring(s) to vector
void readFile(const std::string& filePath, std::vector<std::string>& sampleData, const std::vector<std::string_view> substring)
{
    std::ifstream file (filePath.c_str());

    if (!file)
    {
        // Print an error and exit
        std::cerr << "Uh oh, file could not be opened!" << std::endl;
        exit(1);
    }

    while (file)
    {
        std::string strInput;
        getline(file, strInput);
        for (const std::string_view word : substring) {
            const unsigned long result {strInput.find(word)};

            if (result != std::string::npos){
                sampleData.push_back(strInput);
                continue;
            }
        }
    }
};

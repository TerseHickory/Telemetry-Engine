#include <fstream>
#include <iostream>
#include <span>
#include <stdio.h>
#include <string>
#include <vector>

std::vector<std::string> get_file_data(const std::string& file_path,
                                       const std::vector<std::string_view> substring);
std::vector<std::string_view> parse_data(std::string_view line, const char* delimiters);
std::vector<std::vector<std::string_view>>
create_data_list(const std::vector<std::string_view>& file_data, const char delimiters);

struct cpu {
    std::string name_;
    std::string user_time_;
    std::string nice_time_;
    std::string system_time_;
    std::string idle_time_;
    std::string iowait_time_;
    std::string irq_time_;
    std::string softirq_time_;
    std::string steal_time_;
    std::string speed_;
};

// Returns lines from chosen data file that include specified keyword(s)
std::vector<std::string> get_file_data(const std::string& file_path,
                                       const std::vector<std::string_view>& substring) {
    std::vector<std::string> file_data{};
    std::ifstream file(file_path.c_str());

    if (!file) {
        std::cerr << "Uh oh, file could not be opened!" << '\n';
        exit(1);
    }

    while (file) {
        std::string str_input{};
        getline(file, str_input);
        for (const std::string_view word : substring) {
            const unsigned long result{str_input.find(word)};

            if (result != std::string_view::npos) {
                file_data.push_back(str_input);
                continue;
            }
        }
    }

    return file_data;
};

// Parses string into vector by specified delimiters
std::vector<std::string_view> parse_data(std::string_view line, const char* delimiters) {
    std::vector<std::string_view> tokens{};
    size_t start{0};
    size_t end{line.find(*delimiters)};

    while (end != std::string_view::npos) {
        tokens.push_back(line.substr(start, end - start));
        start = end + 1;
        end = line.find(*delimiters, start);
    }
    tokens.push_back(line.substr(start));

    return tokens;
};

// Creates a multidimensional vector that separates data by values and by line
std::vector<std::vector<std::string_view>>
create_data_list(std::span<const std::string_view> file_data, const char delimiters) {
    std::vector<std::vector<std::string_view>> data_list{};

    for (std::string_view line : file_data) {
        data_list.push_back(parse_data(line, &delimiters));
    }

    return data_list;
};

// Creates a new CPU object using data parsed from "/proc/stat" and "/proc/cpuinfo" for each
// processor
/* cpu create_processors() {

}; */

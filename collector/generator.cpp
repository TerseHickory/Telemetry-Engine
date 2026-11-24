#include <chrono>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

using namespace std::chrono;

struct cpu {
    std::string name_;
    uint32_t user_time_{};
    uint32_t nice_time_{};
    uint32_t system_time_{};
    uint32_t idle_time_{};
    uint32_t iowait_time_{};
    uint32_t irq_time_{};
    uint32_t softirq_time_{};
    uint32_t steal_time_{};
    time_point<system_clock> time_retrieved_ = system_clock::now();
};

std::optional<std::string_view> extract_token(std::string_view line) {
    size_t start{line.find_first_not_of(" \t")};
    if (start == std::string_view::npos) {
        return std::nullopt;
    }

    size_t end{line.find_first_of(" \t", start)};
    std::string_view token =
        (end == std::string_view::npos) ? line.substr(start) : line.substr(start, end - start);

    line = (end == std::string_view::npos) ? std::string_view{} : line.substr(end);
    return token;
};

template <typename T> std::optional<T> get_int(std::optional<std::string_view> token) {
    T value;

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    auto [ptr, ec] = std::from_chars(token->data(), token->data() + token->size(), value);

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    if (ec == std::errc{} && ptr == token->data() + token->size()) {
        return value;
    }

    return std::nullopt;
};

// Parses the /proc/stat file for cpu data
std::optional<cpu> parse_cpu_data(std::string_view line) {
    if (!line.starts_with("cpu")) {
        return std::nullopt;
    }

    std::optional<std::string_view> token = extract_token(line);

    auto name = token;
    auto user_time = get_int<uint32_t>(token);
    auto nice_time = get_int<uint32_t>(token);
    auto system_time = get_int<uint32_t>(token);
    auto idle_time = get_int<uint32_t>(token);
    auto iowait_time = get_int<uint32_t>(token);
    auto irq_time = get_int<uint32_t>(token);
    auto softirq_time = get_int<uint32_t>(token);
    auto steal_time = get_int<uint32_t>(token);

    if (!name || !user_time || !nice_time || !system_time || !idle_time || !iowait_time ||
        !irq_time || !softirq_time || !steal_time) {
        return std::nullopt;
    }

    cpu processor;
    processor.name_ = std::string{*name};
    processor.user_time_ = *user_time;
    processor.nice_time_ = *nice_time;
    processor.system_time_ = *system_time;
    processor.idle_time_ = *idle_time;
    processor.iowait_time_ = *iowait_time;
    processor.irq_time_ = *irq_time;
    processor.softirq_time_ = *softirq_time;
    processor.steal_time_ = *steal_time;

    return processor;
};

std::optional<std::vector<cpu>> load_stat_file(const std::filesystem::path& path) {
    std::ifstream file{path};
    if (!file) {
        std::cerr << "ERROR: cpu data file not found!" << '\n';
        return std::nullopt;
    }

    std::vector<cpu> processors;

    std::string line;

    while (std::getline(file, line)) {
        if (auto record = parse_cpu_data(line)) {
            processors.push_back(std::move(*record));
        }
    }

    return processors;
};

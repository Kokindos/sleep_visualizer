#include "DataLoader.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>

DailySleepData DataLoader::loadFromJsonFile(const std::string &filename) {
    DailySleepData result;

    try {
        std::ifstream ifs(filename);
        if (!ifs.is_open()) {
            std::cerr << "unable to open file: " << filename << std::endl;
            return result;
        }

        nlohmann::json j;
        ifs >> j;

        result.date = parseDateTime(j.value("date", ""), "%Y-%m-%d");
        result.bedtime = parseDateTime(j.value("bedtime", ""));
        result.wakeTime = parseDateTime(j.value("wake_time", ""));

        if (j.contains("phases") && j["phases"].is_array()) {
            for (auto &phase: j["phases"]) {
                //todo try?
                SleepPhase sp;

                //todo заменить на at?
                sp.type = fromString(phase.value("type", ""));
                sp.start = parseDateTime(phase.value("start", ""));
                sp.end = parseDateTime(phase.value("end", ""));
                result.phases.push_back(sp);
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "error parsing JSON: " << e.what() << std::endl;
    }
    return result;
}

std::vector<DailySleepData> DataLoader::loadFromMultipleJsonFiles(const std::vector<std::string> &filenames) {
    std::vector<DailySleepData> allData;
    for (const auto &file: filenames) {

        try {
            DailySleepData data = loadFromJsonFile(file);
            allData.push_back(data);
        }
        catch (const std::exception &e) {
            std::cerr << "error parsing file:" << file;
        }
    }
    return allData;
}

SleepPhaseType DataLoader::fromString(const std::string &phaseStr) {
    if (phaseStr == "Light") return SleepPhaseType::Light;
    if (phaseStr == "Deep") return SleepPhaseType::Deep;
    if (phaseStr == "REM") return SleepPhaseType::REM;
    if (phaseStr == "Awake") return SleepPhaseType::Awake;
    throw std::invalid_argument("invalid sleep phase string");
}

DateTime DataLoader::parseDateTime(const std::string &dateTimeStr, const std::string &format) {
    std::tm tm = {};
    std::istringstream ss(dateTimeStr);
    ss >> std::get_time(&tm, format.c_str());

    if (ss.fail()) {
        throw std::invalid_argument("invalid date format: " + dateTimeStr);
    }
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

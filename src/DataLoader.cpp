#include "DataLoader.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>

WeeklySleepData DataLoader::loadFromJsonFile(const std::string &filename) {
    WeeklySleepData weeklySleepData;

    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw std::runtime_error("unable to open file: " + filename);
    }

    nlohmann::json j;
    ifs >> j;

    try {
        weeklySleepData = DataLoader::parseWeeklyData(j);
    } catch (const std::exception &e) {
        std::cerr << "error parsing JSON: " << e.what() << std::endl;
    }

    return weeklySleepData;
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

DailySleepData DataLoader::parseDailyData(const nlohmann::json &j) {
    DailySleepData result;
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
    return result;
}

WeeklySleepData DataLoader::parseWeeklyData(const nlohmann::json &weeklyJson) {
    WeeklySleepData weeklySleepData;

    if (!weeklyJson.is_array() || weeklyJson.size() != 7) {
        throw std::runtime_error("invalid weekly data format: expected an array of 7 days");
    }

    for (size_t i = 0; i < 7; ++i) {
        try {
            weeklySleepData.sleepDays[i] = parseDailyData(weeklyJson[i]);
        } catch (const std::exception &e) {
            throw std::runtime_error("failed to parse day " + std::to_string(i + 1) + ": " + e.what());
        }
    }
    return weeklySleepData;
}
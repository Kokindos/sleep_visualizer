#include "DataLoader.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

SleepPhaseType fromString(const std::string &phaseStr) {
    if (phaseStr == "Light") return SleepPhaseType::Light;
    if (phaseStr == "Deep") return SleepPhaseType::Deep;
    if (phaseStr == "REM") return SleepPhaseType::REM;
    if (phaseStr == "Awake") return SleepPhaseType::Awake;
    throw std::invalid_argument("Invalid sleep phase string");
}

DailySleepData DataLoader::LoadFromJsonFile(const std::string &filename) {
    DailySleepData result;
    try {
        std::ifstream ifs(filename);
        if (!ifs.is_open()) {
            std::cerr << "unable to open file: " << filename << std::endl;
            return result;
        }

        nlohmann::json j;
        ifs >> j;

        result.date = j.value("date", "");
        result.bedtime = j.value("bedtime", "");
        result.wakeTime = j.value("wake_time", "");

        if (j.contains("phases") && j["phases"].is_array()) {
            for (auto &phase: j["phases"]) {
                SleepPhase sp;

                sp.type = fromString(phase.value("type", ""));
                sp.start = phase.value("start", "");
                sp.end = phase.value("end", "");
                result.phases.push_back(sp);
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "error parsing JSON: " << e.what() << std::endl;
    }
    return result;
}

std::vector<DailySleepData> DataLoader::LoadFromMultipleJsonFiles(const std::vector<std::string> &filenames) {
    std::vector<DailySleepData> allData;
    for (const auto &file: filenames) {

        try {
            DailySleepData data = LoadFromJsonFile(file);
            allData.push_back(data);
        }
        catch (const std::exception &e) {
            std::cerr << "error parsing file:" << file;
        }
    }
    return allData;
}

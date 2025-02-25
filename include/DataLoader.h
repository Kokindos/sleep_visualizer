#ifndef SLEEP_VISUALIZER_DATALOADER_H
#define SLEEP_VISUALIZER_DATALOADER_H


#include <string>
#include <vector>
#include <chrono>
#include "nlohmann/json.hpp"

using DateTime = std::chrono::system_clock::time_point;

enum class SleepPhaseType {
    Light,
    Deep,
    REM,
    Awake
};

struct SleepPhase {
    SleepPhaseType type;
    DateTime start;
    DateTime end;
};

// сон за один день
struct DailySleepData {
    DateTime date;
    DateTime bedtime;
    DateTime wakeTime;
    std::vector<SleepPhase> phases;
//    для будущих визуализаций
//    int averageHeartRate;
//    int respirationRate;
};

struct WeeklySleepData {
    std::array<DailySleepData, 7> sleepDays;
};


class DataLoader {
public:
    static WeeklySleepData loadFromJsonFile(const std::string &filename);

private:
    static DailySleepData parseDailyData(const nlohmann::json &j);

    static SleepPhaseType fromString(const std::string &phaseStr);

    static DateTime parseDateTime(const std::string &dateTimeStr, const std::string &format = "%Y-%m-%d %H:%M");

    static WeeklySleepData parseWeeklyData(const nlohmann::json &weeklyJson);
};

#endif //SLEEP_VISUALIZER_DATALOADER_H

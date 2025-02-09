#ifndef SLEEP_VISUALIZER_DATALOADER_H
#define SLEEP_VISUALIZER_DATALOADER_H


#include <string>
#include <vector>
#include <chrono>

using DateTime = std::chrono::system_clock::time_point;

enum class SleepPhaseType {
    Light,
    Deep,
    REM,
    Awake
};

// фаза сна
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


class DataLoader {
public:
    static DailySleepData loadFromJsonFile(const std::string &filename);

    //todo лучше перейти на один файл
    static std::vector<DailySleepData> loadFromMultipleJsonFiles(const std::vector<std::string> &filenames);

private:
    static SleepPhaseType fromString(const std::string &phaseStr);

    static DateTime parseDateTime(const std::string &dateTimeStr, const std::string &format = "%Y-%m-%d %H:%M");
};

#endif //SLEEP_VISUALIZER_DATALOADER_H

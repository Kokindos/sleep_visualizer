#ifndef SLEEP_VISUALIZER_DATALOADER_H
#define SLEEP_VISUALIZER_DATALOADER_H


#include <string>
#include <vector>
#include <chrono>


enum class SleepPhaseType {
    Light,
    Deep,
    REM,
    Awake
};

SleepPhaseType fromString(const std::string &phaseStr);

// фаза сна
struct SleepPhase {
    SleepPhaseType type;
    std::string start;
    std::string end;
};

// сон за один день
struct DailySleepData {
    std::string date;
    std::string bedtime;
    std::string wakeTime;
    std::vector<SleepPhase> phases;
//    для будущих визуализаций
//    int averageHeartRate;
//    int respirationRate;
};


class DataLoader {
public:

    static DailySleepData LoadFromJsonFile(const std::string &filename);

    //todo лучше перейти на один файл
    static std::vector<DailySleepData> LoadFromMultipleJsonFiles(const std::vector<std::string> &filenames);
};


#endif //SLEEP_VISUALIZER_DATALOADER_H

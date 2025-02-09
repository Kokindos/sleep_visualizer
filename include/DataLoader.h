#ifndef SLEEP_VISUALIZER_DATALOADER_H
#define SLEEP_VISUALIZER_DATALOADER_H


#include <string>
#include <vector>
#include <chrono>

// фаза сна
struct SleepPhase {
    std::string phase;      // "Light", "Deep", "REM", "Awake"
    std::string start;
    std::string end;
};

// сон за один день
struct DailySleepData {
    std::string date;
    std::string bedtime;
    std::string wakeTime;
    std::vector<SleepPhase> phases;
};


class DataLoader {
public:

    static DailySleepData LoadFromJsonFile(const std::string &filename);

    static std::vector<DailySleepData> LoadFromMultipleJsonFiles(const std::vector<std::string> &filenames);
};


#endif //SLEEP_VISUALIZER_DATALOADER_H

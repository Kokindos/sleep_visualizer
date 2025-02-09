#ifndef SLEEP_VISUALIZER_SLEEPANALYZER_H
#define SLEEP_VISUALIZER_SLEEPANALYZER_H

#include <vector>
#include <string>
#include "DataLoader.h"

// метрики сна
struct SleepMetrics {
    double timeInBed;       // время в постели, часы
    double totalSleepTime;  // общее время сна, часы
    double sleepOnset;      // время засыпания, мин
    int awakeningsCount;    // количество пробуждений
    double deepSleep;       // длительность глубоко сна, часы
    double remSleep;        // длительность REM сна, часы
    double lightSleep;      // длительность легкого сна, часы
    double awakeTime;       // общее время бодрствований
    double efficiency;      // метрика эффективности (1..100)
};

class SleepAnalyzer {
public:

    static SleepMetrics CalculateDailyMetrics(const DailySleepData &data);

    static SleepMetrics CalculateAverageMetrics(const std::vector<SleepMetrics> &metrics);

    static double CalculateSleepEfficiency(const SleepMetrics &m);

    static std::vector<SleepMetrics> CalculateMetricsForPeriod(const std::vector<DailySleepData> &data);
};

#endif //SLEEP_VISUALIZER_SLEEPANALYZER_H

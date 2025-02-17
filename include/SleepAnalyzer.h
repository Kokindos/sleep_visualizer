#ifndef SLEEP_VISUALIZER_SLEEPANALYZER_H
#define SLEEP_VISUALIZER_SLEEPANALYZER_H

#include <vector>
#include <string>
#include "DataLoader.h"

struct SleepMetrics {
    int timeInBed;       // время в постели, мин
    int totalSleepTime;  // общее время сна, мин
    int sleepOnset;      // время засыпания, мин
    int awakeningsCount;

    int awakeDuration;
    int deepSleepDuration;
    int remSleepDuration;
    int lightSleepDuration;

    double lightSleepPercent;
    double deepSleepPercent;
    double remSleepPercent;

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

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

struct AverageSleepMetrics {
    double timeInBed;
    double totalSleepTime;
    double sleepOnset;
    double awakeningsCount;

    double awakeDuration;
    double deepSleepDuration;
    double remSleepDuration;
    double lightSleepDuration;

    double lightSleepPercent;
    double deepSleepPercent;
    double remSleepPercent;

    double efficiency;
};

class SleepAnalyzer {
public:

    static SleepMetrics CalculateDailyMetrics(const DailySleepData &data);

    static AverageSleepMetrics CalculateAverageMetrics(const WeeklySleepData &weeklyData);

    static double CalculateSleepEfficiency(const SleepMetrics &m);
};

#endif //SLEEP_VISUALIZER_SLEEPANALYZER_H

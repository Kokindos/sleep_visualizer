#ifndef SLEEP_VISUALIZER_SLEEPANALYZER_H
#define SLEEP_VISUALIZER_SLEEPANALYZER_H

#include <vector>
#include <string>
#include "DataLoader.h"

//для средних метрик за неделю полученные средние значения округляются
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

    static SleepMetrics CalculateAverageMetrics(const WeeklySleepData &weeklyData);

    static double CalculateSleepEfficiency(const SleepMetrics &m);
};

#endif //SLEEP_VISUALIZER_SLEEPANALYZER_H

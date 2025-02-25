#ifndef SLEEP_VISUALIZER_VISUALIZATION_H
#define SLEEP_VISUALIZER_VISUALIZATION_H

#include <vector>
#include <string>
#include "DataLoader.h"
#include "SleepAnalyzer.h"

class Visualization {
public:
    static void ShowDailyPhasesPlot(const DailySleepData &data);

    static void ShowMetricsSummary(const SleepMetrics &m, const bool isAverage);

};

#endif //SLEEP_VISUALIZER_VISUALIZATION_H

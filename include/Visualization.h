#ifndef SLEEP_VISUALIZER_VISUALIZATION_H
#define SLEEP_VISUALIZER_VISUALIZATION_H

#include <vector>
#include <string>
#include "DataLoader.h"
#include "SleepAnalyzer.h"

class Visualization {
public:

    static void ShowDailyPhasesPlot(const DailySleepData &data);

    static void ShowDailySummary(const DailySleepData &data);

    // статистика за период
    static void ShowPeriodMetricsPlot(const std::vector<DailySleepData> &data);

    // зависимость времени отхода ко сну от процента REM
    static void ShowBedtimeVsRemPlot(const std::vector<DailySleepData> &data);

    // ключевые метрики
    static void ShowMetricsTable(const SleepMetrics &metrics);

};

#endif //SLEEP_VISUALIZER_VISUALIZATION_H

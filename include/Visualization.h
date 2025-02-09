#ifndef SLEEP_VISUALIZER_VISUALIZATION_H
#define SLEEP_VISUALIZER_VISUALIZATION_H

#include <vector>
#include <string>
#include "DataLoader.h"
#include "SleepAnalyzer.h"

class Visualization {
public:

    // график фаз сна (столбчатая диаграмма)
    static void ShowDailyPhasesPlot(const DailySleepData &data);

    // статистика за период
    static void ShowPeriodMetricsPlot(const std::vector<DailySleepData> &data);

    // зависимость времени отхода ко сну от процента REM
    static void ShowBedtimeVsRemPlot(const std::vector<DailySleepData> &data);

    // ключевые метрики
    static void ShowMetricsTable(const SleepMetrics &metrics, const std::string &title);
};

#endif //SLEEP_VISUALIZER_VISUALIZATION_H

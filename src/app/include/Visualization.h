#ifndef SLEEP_VISUALIZER_VISUALIZATION_H
#define SLEEP_VISUALIZER_VISUALIZATION_H

#include <vector>
#include <string>
#include "../libraries/sleep_data_loader/DataLoader.h"
#include "SleepAnalyzer.h"

/**
* @brief Класс, строящий графики ImPlot
*/
class Visualization {
public:
    /**
    * @brief Отрисовывает график, визуализирующий таймлан фаз сна за день
    *
    * @param data - DailySleepData - данные о сне за сутки
    */
    static void ShowDailyPhasesPlot(const DailySleepData &data);

    /**
    * @brief Отрисовывает таблицы и графики, отражающие метрики сна за день или неделю
    *
    * @param m - SleepMetrics - посчитанные метрики сна
    * @param isAverage - вид графика: за сутки(false) или за неделю (true)
    */
    static void ShowMetricsSummary(const SleepMetrics &m, bool isAverage);
};

#endif //SLEEP_VISUALIZER_VISUALIZATION_H

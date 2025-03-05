#ifndef SLEEP_VISUALIZER_SLEEPANALYZER_H
#define SLEEP_VISUALIZER_SLEEPANALYZER_H

#include <vector>
#include <string>
#include "../../sleep_data_loader/DataLoader.h"

/**
 * @brief Структура для представления метрик сна.
 *
 * Для средних метрик за неделю полученные средние значения округляются.
 */
struct SleepMetrics {
    int timeInBed;       /**< Время в постели, мин. */
    int totalSleepTime;  /**< Общее время сна, мин. */
    int sleepOnset;      /**< Время засыпания, мин. */
    int awakeningsCount; /**< Количество пробуждений. */

    int awakeDuration;    /**< Продолжительность бодрствования, мин. */
    int deepSleepDuration; /**< Продолжительность глубокого сна, мин. */
    int remSleepDuration;  /**< Продолжительность REM сна, мин. */
    int lightSleepDuration; /**< Продолжительность легкого сна, мин. */

    double lightSleepPercent; /**< Процент легкого сна от общего времени сна. */
    double deepSleepPercent;  /**< Процент глубокого сна от общего времени сна. */
    double remSleepPercent;   /**< Процент REM сна от общего времени сна. */

    double efficiency; /**< Метрика эффективности сна (от 1 до 100). */
};

/**
 * @brief Класс для анализа данных о сне.
 */
class SleepAnalyzer {
public:

    /**
     * @brief Рассчитывает метрики сна за один день.
     *
     * @param data Данные за день
     * @return SleepMetrics Метрики сна
     */
    static SleepMetrics CalculateDailyMetrics(const DailySleepData &data);

    /**
     * @brief Рассчитывает средние метрики сна за неделю.
     *
     * @param weeklyData Данные о сне за неделю.
     * @return SleepMetrics Средние метрики сна.
     */
    static SleepMetrics CalculateAverageMetrics(const WeeklySleepData &weeklyData);

    /**
     * @brief Рассчитывает эффективность сна на основе метрик сна.
     *
     * @param m Метрики сна.
     * @return double Эффективность сна от 1 до 100.
     */
    static double CalculateSleepEfficiency(const SleepMetrics &m);
};


#endif //SLEEP_VISUALIZER_SLEEPANALYZER_H

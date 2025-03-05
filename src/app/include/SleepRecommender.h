#ifndef SLEEP_VISUALIZER_SLEEPRECOMMENDER_H
#define SLEEP_VISUALIZER_SLEEPRECOMMENDER_H

#include <string>
#include "SleepAnalyzer.h"


class SleepRecommender {
public:
    /**
    * @brief Генерирует текстовую рекомендацию, как улучшить сон
    *
    * @param  metrics - данные о сне за любой промежуток времени
    */
    static std::string GenerateRecommendation(const SleepMetrics &metrics);

    /**
    * @brief Сравнивает статистику сна за 2 дня и генерирует текстовое описание сравнения
    *
    */
    static std::string GenerateInsight(const SleepMetrics &today, const SleepMetrics &yesterday);
};


#endif //SLEEP_VISUALIZER_SLEEPRECOMMENDER_H

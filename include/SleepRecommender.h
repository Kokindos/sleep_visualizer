#ifndef SLEEP_VISUALIZER_SLEEPRECOMMENDER_H
#define SLEEP_VISUALIZER_SLEEPRECOMMENDER_H

#include <string>
#include "SleepAnalyzer.h"


class SleepRecommender {
public:

    static std::string GenerateRecommendation(const SleepMetrics &metrics);

    // выводы из данных (сравнение)
    static std::string GenerateInsight(const SleepMetrics &today, const SleepMetrics &yesterday);
};


#endif //SLEEP_VISUALIZER_SLEEPRECOMMENDER_H

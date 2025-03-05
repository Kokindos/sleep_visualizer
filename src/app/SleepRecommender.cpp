#include "SleepRecommender.h"
#include <sstream>

std::string SleepRecommender::GenerateRecommendation(const SleepMetrics &m) {

    std::ostringstream oss;
    oss << "Рекомендации:\n";

    if (m.totalSleepTime < 7.0) {
        oss << "- Старайтесь увеличить общее время сна, и перечислить рекомендации.\n";
    }
    if (m.remSleepDuration < 1.0) {
        oss << "- Рекомендации, чтобы улучшить длительность REM-фазы.\n";
    }
    if (m.deepSleepDuration < 1.5) {
        oss
                << "- рекомендации, чтобы улучшить глубокий сон.\n";
    }
    if (m.efficiency < 70.0) {
        oss
                << "- У вас низкая эффективность сна. Рекомендации.\n";
    }

    if (oss.str() == "Рекомендации:\n") {
        oss << "- Ваш сон в порядке!\n";
    }

    return oss.str();
}

std::string SleepRecommender::GenerateInsight(const SleepMetrics &today, const SleepMetrics &yesterday) {
    double diff = today.totalSleepTime - yesterday.totalSleepTime;

    std::ostringstream oss;
    if (diff > 0.0) {
        oss << "Сегодня вы спали на " << diff << " ч. больше, чем вчера.\n";
    } else if (diff < 0.0) {
        oss << "Сегодня вы спали на " << (-diff) << " ч. меньше, чем вчера.\n";
    } else {
        oss << "Сегодня вы спали столько же, сколько и вчера.\n";
    }

    return oss.str();
}

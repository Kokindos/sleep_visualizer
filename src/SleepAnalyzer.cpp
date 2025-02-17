#include "SleepAnalyzer.h"
#include "DateUtils.h"

SleepMetrics SleepAnalyzer::CalculateDailyMetrics(const DailySleepData &data) {
    //todo убрать высчитываемые поля
    SleepMetrics m{};

    const int timeInBed = DateUtils::diffBetween(data.bedtime, data.wakeTime);
    m.timeInBed = timeInBed;

    int awakeCount = 0;

    for (const auto &phase: data.phases) {
        int phaseDuration = DateUtils::diffBetween(phase.start, phase.end);

        if (phase.type == SleepPhaseType::Awake) {
            m.awakeDuration += phaseDuration;
            m.awakeningsCount++;
        } else if (phase.type == SleepPhaseType::Light) {
            m.lightSleepDuration += phaseDuration;
        } else if (phase.type == SleepPhaseType::Deep) {
            m.deepSleepDuration += phaseDuration;
        } else if (phase.type == SleepPhaseType::REM) {
            m.remSleepDuration += phaseDuration;
        }
    }

    const int totalSleepTimeMinutes = m.lightSleepDuration + m.deepSleepDuration + m.remSleepDuration;
    m.totalSleepTime = totalSleepTimeMinutes;
    m.lightSleepPercent = (double) m.lightSleepDuration / totalSleepTimeMinutes * 100.0;
    m.deepSleepPercent = (double) m.deepSleepDuration / totalSleepTimeMinutes * 100.0;
    m.remSleepPercent = (double) m.remSleepDuration / totalSleepTimeMinutes * 100.0;

    m.awakeningsCount = awakeCount;
    //todo разница между bedtime и началом первой  не awake фазы сна
    m.sleepOnset = 123;
    m.efficiency = CalculateSleepEfficiency(m);

    return m;
}

std::vector<SleepMetrics> SleepAnalyzer::CalculateMetricsForPeriod(const std::vector<DailySleepData> &data) {
    std::vector<SleepMetrics> metricsList;
    metricsList.reserve(data.size());
    for (const auto &dayData: data) {
        metricsList.push_back(CalculateDailyMetrics(dayData));
    }
    return metricsList;
}

double SleepAnalyzer::CalculateSleepEfficiency(const SleepMetrics &m) {

    // 100*(totalSleepTime/timeInBed)-(0.5*awakeningsCount)-(sleepOnset/60)
    // ограничим снизу 1, сверху 100
    if (m.timeInBed <= 0) return 1.0;

    double ratio = m.totalSleepTime / m.timeInBed;
    double base = 100.0 * ratio;
    double penaltyAwakenings = 0.5 * m.awakeningsCount;
    double penaltyOnset = m.sleepOnset / 60.0; // в часах
    double efficiency = base - penaltyAwakenings - penaltyOnset;
    if (efficiency < 1.0) efficiency = 1.0;
    if (efficiency > 100.0) efficiency = 100.0;
    return efficiency;
}

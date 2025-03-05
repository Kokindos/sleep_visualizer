#include "SleepAnalyzer.h"
#include "DateUtils.h"
#include <numeric>

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
            awakeCount++;
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

SleepMetrics SleepAnalyzer::CalculateAverageMetrics(const WeeklySleepData &weeklyData) {
    const auto totalMetrics = std::accumulate(
            weeklyData.sleepDays.begin(),
            weeklyData.sleepDays.end(),
            SleepMetrics{},
            [](SleepMetrics acc, const DailySleepData &day) {
                SleepMetrics daily = CalculateDailyMetrics(day);
                acc.timeInBed += daily.timeInBed;
                acc.awakeDuration += daily.awakeDuration;
                acc.lightSleepDuration += daily.lightSleepDuration;
                acc.deepSleepDuration += daily.deepSleepDuration;
                acc.remSleepDuration += daily.remSleepDuration;
                acc.totalSleepTime += daily.totalSleepTime;
                acc.efficiency += daily.efficiency;
                acc.sleepOnset += daily.sleepOnset;
                acc.awakeningsCount += daily.awakeningsCount;
                return acc;
            }
    );

    SleepMetrics avgMetrics = {};

    const auto daysCount = static_cast<double>(weeklyData.sleepDays.size());
    auto avg = [daysCount](int total) {
        return static_cast<int>(std::round((total) / daysCount));
    };

    const int totalSleepTime = totalMetrics.totalSleepTime;
    auto calculatePercentage = [totalSleepTime](int duration) {
        return static_cast<double>(duration) / totalSleepTime * 100.0;
    };

    avgMetrics.timeInBed = avg(totalMetrics.timeInBed);
    avgMetrics.awakeDuration = avg(totalMetrics.awakeDuration);
    avgMetrics.lightSleepDuration = avg(totalMetrics.lightSleepDuration);
    avgMetrics.deepSleepDuration = avg(totalMetrics.deepSleepDuration);
    avgMetrics.remSleepDuration = avg(totalMetrics.remSleepDuration);
    avgMetrics.totalSleepTime = avg(totalMetrics.totalSleepTime);
    avgMetrics.efficiency = totalMetrics.efficiency / daysCount;
    avgMetrics.sleepOnset = avg(totalMetrics.sleepOnset);
    avgMetrics.awakeningsCount = avg(totalMetrics.awakeningsCount);

    avgMetrics.lightSleepPercent = calculatePercentage(totalMetrics.lightSleepDuration);
    avgMetrics.deepSleepPercent = calculatePercentage(totalMetrics.deepSleepDuration);
    avgMetrics.remSleepPercent = calculatePercentage(totalMetrics.remSleepDuration);

    return avgMetrics;
}

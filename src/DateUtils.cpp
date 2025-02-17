#include <chrono>
#include <string>
#include "DateUtils.h"

std::string DateUtils::onlyTime(const std::chrono::system_clock::time_point &tp) {
    return std::format("{:%H:%M}", tp);
}

std::string DateUtils::onlyDate(const std::chrono::system_clock::time_point &tp) {
    return std::format("{:%Y-%m-%d}", tp);
}

std::chrono::system_clock::time_point DateUtils::unixToTimePoint(const double &unixTime) {
    return std::chrono::system_clock::time_point(std::chrono::milliseconds(static_cast<int64_t>(unixTime * 1000)));
}

double DateUtils::timePointToUnix(const std::chrono::system_clock::time_point &tp) {
    auto duration = std::chrono::duration<double>(tp.time_since_epoch());
    return duration.count();
}

int DateUtils::diffBetween(const std::chrono::system_clock::time_point &start,
                           const std::chrono::system_clock::time_point &end) {

    return std::chrono::duration_cast<std::chrono::minutes>(end - start).count();
}

std::string DateUtils::formatTimeDiff(const int &timeDiff) {
    int hours = timeDiff / 60;
    int minutes = timeDiff % 60;
    return std::to_string(hours) + " ч. " + std::to_string(minutes) + " мин.";
}


#ifndef SLEEPVISUALIZER_DATE_FORMATTER_H
#define SLEEPVISUALIZER_DATE_FORMATTER_H

#include <chrono>
#include <string>

class DateUtils {
public:
    DateUtils() = delete;

    DateUtils(const DateUtils &) = delete;

    DateUtils &operator=(const DateUtils &) = delete;

    //todo возможно возвращать c_str()
    static std::string onlyTime(const std::chrono::system_clock::time_point &tp);

    static std::string onlyDate(const std::chrono::system_clock::time_point &tp);

    static std::chrono::system_clock::time_point unixToTimePoint(const double &unixTime);

    static double timePointToUnix(const std::chrono::system_clock::time_point &tp);
};

#endif //SLEEPVISUALIZER_DATE_FORMATTER_H

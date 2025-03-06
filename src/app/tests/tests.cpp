#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "../libraries/date_utils/DateUtils.h"
#include <chrono>
#include <chrono>
#include <format>

using namespace std::chrono;
using namespace std::chrono_literals;

TEST_CASE("Testing onlyDate function") {
    SUBCASE("Valid date formatting") {
        std::chrono::system_clock::time_point tp = sys_days{2024y / 5 / 31};
        CHECK(DateUtils::onlyDate(tp) == "2024-05-31");

        tp = sys_days{2000y / 1 / 1} + 12h + 30min;
        CHECK(DateUtils::onlyDate(tp) == "2000-01-01");

        tp = sys_days{1999y / 12 / 31} + 23h + 59min + 59s;
        CHECK(DateUtils::onlyDate(tp) == "1999-12-31");
    }
}

TEST_CASE("Testing onlyTime function") {
    SUBCASE("Valid time formatting") {
        auto tp = sys_days{2024y / 5 / 31} + 12h + 30min;
        CHECK(DateUtils::onlyTime(tp)
              == "12:30");

        tp = sys_days{2024y / 5 / 31} + 23h + 59min;
        CHECK(DateUtils::onlyTime(tp)
              == "23:59");
    }
}

TEST_CASE("Testing unixToTimePoint and timePointToUnix") {
    SUBCASE("Roundtrip conversion") {
        double original = 1717056000.0; // 2024-05-31 00:00:00 UTC
        auto tp = DateUtils::unixToTimePoint(original);
        double converted = DateUtils::timePointToUnix(tp);
        CHECK(converted
              ==
              doctest::Approx(original)
                      .epsilon(0.001));
    }

    SUBCASE("Specific timestamp check") {
        auto tp = DateUtils::unixToTimePoint(0.0);
        CHECK(DateUtils::onlyDate(tp)
              == "1970-01-01");
        CHECK(DateUtils::onlyTime(tp)
              == "00:00");
    }
}

TEST_CASE("Testing diffBetween function") {
    SUBCASE("Positive difference") {
        auto tp1 = sys_days{2024y / 5 / 31} + 10h + 30min;
        auto tp2 = tp1 + 2h + 15min;
        CHECK(DateUtils::diffBetween(tp1, tp2)
              == 135);
    }

    SUBCASE("Negative difference") {
        auto tp1 = sys_days{2024y / 5 / 31} + 15h;
        auto tp2 = tp1 - 3h;
        CHECK(DateUtils::diffBetween(tp1, tp2)
              == -180);
    }
}

TEST_CASE("Testing formatTimeDiff function") {
    SUBCASE("Regular cases") {
        CHECK(DateUtils::formatTimeDiff(90)
              == "1 ч. 30 мин.");
        CHECK(DateUtils::formatTimeDiff(61)
              == "1 ч. 1 мин.");
    }

    SUBCASE("Edge cases") {
        CHECK(DateUtils::formatTimeDiff(0)
              == "0 ч. 0 мин.");
        CHECK(DateUtils::formatTimeDiff(1500)
              == "25 ч. 0 мин.");
    }
}
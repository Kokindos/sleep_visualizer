#ifndef SLEEPVISUALIZER_DATE_FORMATTER_H
#define SLEEPVISUALIZER_DATE_FORMATTER_H

#include <chrono>
#include <string>

/**
 * @brief Утилиты для обработки и форматирования даты и времени.
 *
 * Этот класс предоставляет статические методы для конвертации между разными представлениями времени
 * и форматирования.
 * Объекты этого класса создавать нельзя.
 */
class DateUtils {
public:
    DateUtils() = delete;

    DateUtils(const DateUtils &) = delete;

    DateUtils &operator=(const DateUtils &) = delete;

    /**
    * @brief Форматирует время в строку, содердащую только часы и минуты
    *
    * @param tp Временная точка, которую нужно отформатировать.
    * @return Строка, представляющая время в формате ЧЧ:ММ.
    */
    //todo возможно возвращать c_str()
    static std::string onlyTime(const std::chrono::system_clock::time_point &tp);

    /**
     * @brief Форматирует время в строку только с датой.
     *
     * @param tp Временная точка, которую нужно отформатировать.
     * @return Строка с представленной датой в формате (YYYY-MM-DD).
     */
    static std::string onlyDate(const std::chrono::system_clock::time_point &tp);

    /**
     * @brief Конвертирует Unix timestamp в time_point.
     *
     * @param unixTime Unix-время, представленное в виде числа с плавающей точкой.
     * @return Соответствующий time_point объект.
     */
    static std::chrono::system_clock::time_point unixToTimePoint(const double &unixTime);

    /**
     * @brief Конвертирует объект time_point в Unix timestamp.
     *
     * @param tp Временная точка
     * @return Unix-время в виде числа с плавающей точкой.
     */
    static double timePointToUnix(const std::chrono::system_clock::time_point &tp);

    /**
    * @brief Возвращает разницу в минутах между двумя временными точками.
    *
    * @param start Начальная временная точка.
    * @param end Конечная временная точка.
    * @return Разница между @p end и @p start в минутах.
    */
    static int diffBetween(const std::chrono::system_clock::time_point &start,
                           const std::chrono::system_clock::time_point &end);


    /**
    * @brief Форматирует целочисленную разницу во времени в минутах в удобочитаемую строку.
    *
    * @param timeDiff Разница во времени в минутах.
    * @return Стнрока вида X ч. Y мин.
    */
    static std::string formatTimeDiff(const int &timeDiff);
};

#endif //SLEEPVISUALIZER_DATE_FORMATTER_H

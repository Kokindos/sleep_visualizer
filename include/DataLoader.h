/**
 * @file DataLoader.cpp
 * @brief Загрузчик данных о сне из JSON файла и парсинга данных.
 */
#ifndef SLEEP_VISUALIZER_DATALOADER_H
#define SLEEP_VISUALIZER_DATALOADER_H

#include <string>
#include <vector>
#include <chrono>
#include "nlohmann/json.hpp"

/**
 * @typedef DataTime
 * @brief Для удобного представления типа даты
 */
using DateTime = std::chrono::system_clock::time_point;

/**
 * @enum SleepPhaseType
 * @brief Перечисления возможных типов фаз сна
 */
enum class SleepPhaseType {
    Light,  ///< Лёгкий сон
    Deep,   ///< Глубокий сон
    REM,    ///< Быстрый сон
    Awake   ///< Бодрствование или пробуждение
};

/**
 * @struct SleepPhase
 * @brief Структура, содержащая информацию о фазе сна
 */
struct SleepPhase {
    SleepPhaseType type; ///< Тип фазы сна
    DateTime start;      ///< Время начала фазы сна
    DateTime end;        ///< Время окончания фазы сна
};

/**
 * @struct DailySleepData
 * @brief Структура для хранения информации о сне за одни сутки.
*/
struct DailySleepData {
    DateTime date;                  ///< Дата, соответствующая данным о сне
    DateTime bedtime;               ///< Время отхода ко сну
    DateTime wakeTime;              ///< Время пробуждения в этот день
    std::vector<SleepPhase> phases; ///< Список фаз сна за ночь
//    для будущих визуализаций
//    int averageHeartRate;
//    int respirationRate;
};

/**
 * @brief Структура для хранения информации о сне за неделю.
 */
struct WeeklySleepData {
    std::array<DailySleepData, 7> sleepDays;
};

/**
 * @class DataLoader
 * @brief Класс для загрузки и парсинга информации о сне из JSON-файлов.
 */
class DataLoader {
public:

    /**
     * @brief Загружает данные о сне за неделю из указанного JSON-файла.
     *
     * @param filename Путь к JSON-файлу, содержащему данные о сне.
     * @return Структура с данными о сне за неделю.
     *
     * @throws std::runtime_error Если невозможно открыть файл или возникла ошибка в процессе парсинга JSON.
     */

    static WeeklySleepData loadFromJsonFile(const std::string &filename);

private:

    /**
     * @brief Парсит JSON-объект с данными сна за один день.
     *
     * @param j JSON-объект с дневными данными о сне.
     * @return DailySleepData - данные сна за один день.
     *
     * @throws std::invalid_argument Если формат какой-либо из строк даты или времени некорректен.
     */
    static DailySleepData parseDailyData(const nlohmann::json &j);

    /**
     * @brief Конвертирует строку с фазой сна в соответствующий enum.
     *
     * @param phaseStr Строковое описание фазы ("Light", "Deep", "REM", "Awake").
     * @return SleepPhaseType Значение enum соответствующей фазы сна.
     *
     * @throws std::invalid_argument Если передана некорректная строка с фазой сна.
     */
    static SleepPhaseType fromString(const std::string &phaseStr);

    /**
     * @brief Парсит дату и время из строки по заданному формату.
     *
     * @param dateTimeStr Строка с датой и временем.
     * @param format Формат парсинга (например, "%Y-%m-%d %H:%M:%S").
     * @return DateTime Преобразованные в DateTime дату и время.
     *
     * @throws std::invalid_argument Если формат строки некорректный или парсинг даты не удался.
     */
    static DateTime parseDateTime(const std::string &dateTimeStr, const std::string &format = "%Y-%m-%d %H:%M");

    /**
     * @brief Парсит JSON-массив с данными за неделю.
     *
     * @param weeklyJson JSON-массив из 7 элементов.
     * @return WeeklySleepData - данные сна за неделю.
     *
     * @throws std::runtime_error Если формат JSON-массива некорректный или число элементов не равно семи, если произошла ошибка парсинга.
     */
    static WeeklySleepData parseWeeklyData(const nlohmann::json &weeklyJson);
};

#endif //SLEEP_VISUALIZER_DATALOADER_H

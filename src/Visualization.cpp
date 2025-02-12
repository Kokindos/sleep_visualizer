#include "Visualization.h"
#include "DateUtils.h"
#include <implot.h>
#include <imgui.h>
#include <string>

//Преобразует time_point в double (кол-во секунд с 1.01.1970)
static double TimePointToUnix(const std::chrono::system_clock::time_point &tp) {
    auto duration = std::chrono::duration<double>(tp.time_since_epoch());
    return duration.count();
}

//Сопоставление имени фазы с “уровнем” по оси Y
static double PhaseToY(const SleepPhaseType &phase) {
    switch (phase) {
        case SleepPhaseType::Awake:
            return 1.0;
        case SleepPhaseType::Light:
            return 2.0;
        case SleepPhaseType::Deep:
            return 3.0;
        case SleepPhaseType::REM:
            return 4.0;
        default:
            return 0.0;
    }
}

static const char *phaseToString(const SleepPhaseType &phase) {
    switch (phase) {
        case SleepPhaseType::Awake:
            return "Awake";
        case SleepPhaseType::Light:
            return "Light";
        case SleepPhaseType::Deep:
            return "Deep";
        case SleepPhaseType::REM:
            return "REM";
        default:
            return "Unknown";
    }
}

static ImVec4 phaseToColor(const SleepPhaseType &phase) {
    switch (phase) {
        case SleepPhaseType::Awake:
            return {1.0f, 0.0f, 0.0f, 1.0f};  // красный
        case SleepPhaseType::Light:
            return {0.0f, 1.0f, 0.0f, 1.0f};  // зелёный
        case SleepPhaseType::Deep:
            return {0.0f, 0.0f, 1.0f, 1.0f}; // синий
        case SleepPhaseType::REM:
            return {1.0f, 1.0f, 0.0f, 1.0f}; // жёлтый
        default:
            return {0.5f, 0.5f, 0.5f, 1.0f}; // серый
    }
}

void Visualization::ShowDailyPhasesPlot(const DailySleepData &data) {
    if (data.phases.empty())
        return;

    ImVec2 plotSize = ImVec2(ImGui::GetContentRegionAvail().x, 300);

    if (ImPlot::BeginPlot("Визуализация фаз сна за день", plotSize, ImPlotFlags_NoInputs)) {
        ImPlot::SetupAxis(ImAxis_X1, "Время");
        ImPlot::SetupAxis(ImAxis_Y1, "Фаза");

        static double yTicks[] = {1.0, 2.0, 3.0, 4.0};
        static const char *yLabels[] = {"Awake", "Light", "Deep", "REM"};
        ImPlot::SetupAxisTicks(ImAxis_Y1, yTicks, 4, yLabels);

        double startTime = TimePointToUnix(data.bedtime);
        double endTime = TimePointToUnix(data.wakeTime);

        ImPlot::SetupAxesLimits(
                startTime,
                endTime,
                0.0, 4.5
        );
        ImPlot::SetupAxisLimitsConstraints(
                ImAxis_Y1, 0, 4.5
        );
        ImPlot::SetupAxisFormat(ImAxis_X1, [](double value, char *buff, int size, void *user_data) -> int {
            auto tp = DateUtils::unixToTimePoint(value);
            std::string formatted = DateUtils::onlyTime(tp);
            std::strncpy(buff, formatted.c_str(), sizeof(buff));
            return std::strlen(buff);
        });


        std::vector<double> xTicks;

        xTicks.push_back(TimePointToUnix(data.phases.front().start));


        for (const auto &phase: data.phases) {
            const DateTime currentPhaseEnd = phase.end;
            double xEnd = TimePointToUnix(currentPhaseEnd);

            xTicks.push_back(xEnd);
        }

        ImPlot::SetupAxisTicks(ImAxis_X1, xTicks.data(), xTicks.size(), nullptr);

        for (const auto &phase: data.phases) {

            //todo не переводить два раза в юникс
            double xStart = TimePointToUnix(phase.start);
            double xEnd = TimePointToUnix(phase.end);
            double yVal = PhaseToY(phase.type);

            double xs[2] = {xStart, xEnd};
            double ys[2] = {yVal, yVal};

            ImPlot::PushStyleColor(ImPlotCol_Line, phaseToColor(phase.type));
            ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 8.0f);
            ImPlot::PlotLine(phaseToString(phase.type), xs, ys, 2);

            ImPlot::PopStyleVar();
            ImPlot::PopStyleColor();
        }

        ImPlot::EndPlot();
    }
}

void Visualization::ShowPeriodMetricsPlot(const std::vector<DailySleepData> &data) {
    // Для наглядности построим график общей длительности сна по датам
    if (data.empty()) return;

    // Сформируем вектор пар (x=индекс, y=totalSleepTime)
    std::vector<double> x(data.size()), y(data.size());

    // Считаем метрики на каждый день
    std::vector<SleepMetrics> metricsVec;
    metricsVec.reserve(data.size());
    for (const auto &d: data) {
        metricsVec.push_back(SleepAnalyzer::CalculateDailyMetrics(d));
    }

    for (int i = 0; i < (int) data.size(); i++) {
        x[i] = i;
        y[i] = metricsVec[i].totalSleepTime;
    }

    if (ImPlot::BeginPlot("График длительности сна за период", ImVec2(-1, 300))) {
        ImPlot::SetupAxes("День (индекс)", "Часы сна", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotLine("Total Sleep Time", x.data(), y.data(), (int) data.size());
        ImPlot::EndPlot();
    }
}

//void Visualization::ShowBedtimeVsRemPlot(const std::vector<DailySleepData> &data) {
//    if (data.empty()) return;
//
//    // x = время отхода ко сну (в часах)
//    // y = % REM (отношение REM ко всему времени сна)
//    std::vector<double> x, y;
//    x.reserve(data.size());
//    y.reserve(data.size());
//
//    for (const auto &d: data) {
//        SleepMetrics m = SleepAnalyzer::CalculateDailyMetrics(d);
//
//        // Время отхода (bedtime) переведём в часы
//        int bedtimeMin = 0;
//        if (!d.bedtime.empty()) {
//            // простая функция перевода
//            int h = 0, min = 0;
//            char sep;
//            std::stringstream ss(d.bedtime);
//            ss >> h >> sep >> min;
//            bedtimeMin = h * 60 + min;
//        }
//        // Для оси X переведём в часы
//        double bedtimeHours = bedtimeMin / 60.0;
//
//        // % REM
//        double total = m.lightSleep + m.deepSleep + m.remSleep;
//        double remPercent = 0.0;
//        if (total > 0.0) {
//            remPercent = (m.remSleep / total) * 100.0;
//        }
//
//        x.push_back(bedtimeHours);
//        y.push_back(remPercent);
//    }
//
//    if (ImPlot::BeginPlot("Зависимость времени отхода ко сну от % REM", ImVec2(-1, 300))) {
//        ImPlot::SetupAxes("Bedtime, ч", "REM, %", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
//        ImPlot::PlotScatter("Points", x.data(), y.data(), (int) x.size());
//        ImPlot::EndPlot();
//    }
//}

void Visualization::ShowMetricsTable(const SleepMetrics &metrics, const std::string &title) {
    ImGui::Text("%s", title.c_str());
    ImGui::Separator();
    ImGui::Text("Время в постели: %.2f ч.", metrics.timeInBed);
    ImGui::Text("Общее время сна: %.2f ч.", metrics.totalSleepTime);
    ImGui::Text("Время засыпания: %.1f мин.", metrics.sleepOnset);
    ImGui::Text("Кол-во пробуждений: %d", metrics.awakeningsCount);
    ImGui::Text("Глубокий сон: %.2f ч.", metrics.deepSleep);
    ImGui::Text("REM сон: %.2f ч.", metrics.remSleep);
    ImGui::Text("Лёгкий сон: %.2f ч.", metrics.lightSleep);
    ImGui::Text("Время бодрствования: %.2f ч.", metrics.awakeTime);
    ImGui::Text("Эффективность сна: %.1f", metrics.efficiency);
    ImGui::Separator();
}

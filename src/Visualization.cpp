#include "Visualization.h"
#include "DateUtils.h"
#include <implot.h>
#include <imgui.h>
#include <string>
#include <unordered_map>

struct PhaseVisualInfo {
    double yLevel;
    ImVec4 color;
    const char *name;

    static const PhaseVisualInfo &getInfo(SleepPhaseType phase) {
        static const std::unordered_map<SleepPhaseType, PhaseVisualInfo> phaseMap = {
                {SleepPhaseType::Awake, {1.0, {1.0f, 0.0f, 0.0f, 1.0f}, "Awake"}},
                {SleepPhaseType::Light, {2.0, {0.0f, 1.0f, 0.0f, 1.0f}, "Light"}},
                {SleepPhaseType::Deep,  {3.0, {0.0f, 0.0f, 1.0f, 1.0f}, "Deep"}},
                {SleepPhaseType::REM,   {4.0, {1.0f, 1.0f, 0.0f, 1.0f}, "REM"}},
        };
        static const PhaseVisualInfo defaultInfo = {0.0, {0.5f, 0.5f, 0.5f, 1.0f}, "Unknown"};

        auto it = phaseMap.find(phase);
        return (it != phaseMap.end()) ? it->second : defaultInfo;
    }
};


void Visualization::ShowDailyPhasesPlot(const DailySleepData &data) {
    if (data.phases.empty()) return;

    ImVec2 plotSize = ImVec2(ImGui::GetContentRegionAvail().x, 300);

    if (ImPlot::BeginPlot("Визуализация фаз сна за день", plotSize, ImPlotFlags_NoInputs)) {
        ImPlot::SetupAxis(ImAxis_X1, "Время");
        ImPlot::SetupAxis(ImAxis_Y1, "Фаза");

        static double yTicks[] = {1.0, 2.0, 3.0, 4.0};
        static const char *yLabels[] = {"Awake", "Light", "Deep", "REM"};
        ImPlot::SetupAxisTicks(ImAxis_Y1, yTicks, 4, yLabels);

        double startTime = DateUtils::timePointToUnix(data.bedtime);
        double endTime = DateUtils::timePointToUnix(data.wakeTime);

        ImPlot::SetupAxesLimits(startTime, endTime, 0.0, 4.5);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, 4.5);

        ImPlot::SetupAxisFormat(ImAxis_X1, [](double value, char *buff, int size, void *user_data) -> int {
            auto tp = DateUtils::unixToTimePoint(value);
            std::string formatted = DateUtils::onlyTime(tp);
            std::strncpy(buff, formatted.c_str(), sizeof(buff));
            return std::strlen(buff);
        });


        std::vector<double> xTicks;
        xTicks.push_back(DateUtils::timePointToUnix(data.phases.front().start));
        for (const auto &phase: data.phases) {
            const DateTime currentPhaseEnd = phase.end;
            double xEnd = DateUtils::timePointToUnix(currentPhaseEnd);
            xTicks.push_back(xEnd);
        }

        ImPlot::SetupAxisTicks(ImAxis_X1, xTicks.data(), xTicks.size(), nullptr);

        for (const auto &phase: data.phases) {
            //todo не переводить два раза в юникс
            double xStart = DateUtils::timePointToUnix(phase.start);
            double xEnd = DateUtils::timePointToUnix(phase.end);

            const auto &info = PhaseVisualInfo::getInfo(phase.type);

            double xs[2] = {xStart, xEnd};
            double ys[2] = {info.yLevel, info.yLevel};

            ImPlot::PushStyleColor(ImPlotCol_Line, info.color);
            ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 8.0f);
            ImPlot::PlotLine(info.name, xs, ys, 2);

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

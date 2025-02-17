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

struct PhaseDurationInfo {
    const char *name;
    int minutes;
    double percentage;
};


void Visualization::ShowDailyPhasesPlot(const DailySleepData &data) {
    if (data.phases.empty()) return;

    ImGui::Text("Дата %s:", DateUtils::onlyDate(data.date).c_str());

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

        ImPlot::SetupAxisTicks(ImAxis_X1, xTicks.data(), (int) xTicks.size(), nullptr);

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

void Visualization::ShowDailySummary(const DailySleepData &data) {
    SleepMetrics m = SleepAnalyzer::CalculateDailyMetrics(data);

    std::array<PhaseDurationInfo, 3> phases = {{{"Light", m.lightSleepDuration, m.lightSleepPercent},
                                                {"Deep", m.deepSleepDuration, m.deepSleepPercent},
                                                {"REM", m.remSleepDuration, m.remSleepPercent}}};

    ImGui::Begin("Статистика за день");

    ImGui::Text("Длительность фаз сна (время и проценты):");
    ImGui::Separator();

    if (ImGui::BeginTable("DurationsTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Фаза");
        ImGui::TableSetupColumn("Длительность");
        ImGui::TableSetupColumn("Доля");
        ImGui::TableHeadersRow();

        for (auto &p: phases) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(p.name);

            ImGui::TableSetColumnIndex(1);
            //todo возможно форматировать через встроенный форматтер, а не самописный
            ImGui::Text("%s", DateUtils::formatTimeDiff(p.minutes).c_str());

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.1f %%", p.percentage);
        }

        ImGui::EndTable();
    }

    if (ImPlot::BeginPlot("Доля каждой фазы", ImVec2(-1, 200))) {
        ImPlot::SetupAxes("Фаза", "Минуты");

        //todo выделять память на тики в других графиках
        std::vector<double> yMinutes(phases.size());
        std::vector<const char *> xLabels(phases.size());
        std::vector<double> xIndices(phases.size());

        for (int i = 0; i < phases.size(); i++) {
            yMinutes[i] = phases[i].minutes;
            xLabels[i] = phases[i].name;
            xIndices[i] = (double) i;
        }

        ImPlot::SetupAxisTicks(ImAxis_X1, xIndices.data(), (int) xIndices.size(), xLabels.data());

        ImPlot::PlotBars("Фазы", xIndices.data(), yMinutes.data(), (int) phases.size(), 0.5);
        ImPlot::EndPlot();
    }

    if (ImPlot::BeginPlot("Доля каждой фазы pie chart", ImVec2(-1, 200),
                          ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText)) {

        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoTickLabels);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, 1);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, 1);

        std::vector<double> percentages(phases.size());
        std::vector<const char *> labels(phases.size());

        for (int i = 0; i < phases.size(); i++) {
            percentages[i] = phases[i].percentage;
            labels[i] = phases[i].name;
        }

        ImPlot::PlotPieChart(labels.data(), percentages.data(), (int) phases.size(),
                             0.5, 0.5, 0.4, "%.1f %%", 90.0);
        ImPlot::EndPlot();
    }


    ImGui::Text("Время в постели: %.2d ч", m.timeInBed);
    ImGui::Text("Общее время сна: %.2d ч", m.totalSleepTime);
    ImGui::Text("Количество пробуждений: %d", m.awakeningsCount);

    ImGui::End();

}

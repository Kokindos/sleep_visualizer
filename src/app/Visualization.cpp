#include "Visualization.h"
#include "DateUtils.h"
#include "SleepAnalyzer.h"
#include "implot.h"
#include "imgui.h"
#include <string>
#include <unordered_map>

struct PhaseVisualInfo {
    double yLevel;
    ImVec4 color;
    const char *name;

    static const std::vector<PhaseVisualInfo> &getPhasesInfo() {
        static const std::vector<PhaseVisualInfo> allPhasesInfo = {
                {1.0, {1.0f,  0.42f, 0.42f, 1.0f}, "Awake"},
                {2.0, {0.42f, 0.79f, 0.47f, 1.0f}, "Light"},
                {3.0, {0.30f, 0.59f, 1.0f,  1.0f}, "Deep"},
                {4.0, {0.65f, 0.42f, 1.0f,  1.0f}, "REM"}
        };
        return allPhasesInfo;
    }

    static const PhaseVisualInfo &getPhaseInfo(SleepPhaseType phase) {
        static const std::unordered_map<SleepPhaseType, PhaseVisualInfo> phaseMap = {
                {SleepPhaseType::Awake, getPhasesInfo()[0]},
                {SleepPhaseType::Light, getPhasesInfo()[1]},
                {SleepPhaseType::Deep,  getPhasesInfo()[2]},
                {SleepPhaseType::REM,   getPhasesInfo()[3]}
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

    ImVec2 windowSize = {ImGui::GetIO().DisplaySize.x, 300};
    ImGui::SetNextWindowPos({0, 30}, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::Begin("Sleep Visualizer", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Дата %s:", DateUtils::onlyDate(data.date).c_str());

    if (ImPlot::BeginPlot("Визуализация фаз сна за день", ImGui::GetContentRegionAvail(), ImPlotFlags_NoInputs)) {
        ImPlot::SetupAxis(ImAxis_X1, "Время");
        ImPlot::SetupAxis(ImAxis_Y1, "Фаза");

        const auto &allPhasesInfo = PhaseVisualInfo::getPhasesInfo();
        std::vector<double> yTicks;
        std::vector<const char *> yLabels;
        for (const auto &phase: allPhasesInfo) {
            yTicks.push_back(phase.yLevel);
            yLabels.push_back(phase.name);
        }
        ImPlot::SetupAxisTicks(ImAxis_Y1, yTicks.data(), static_cast<int>(yTicks.size()), yLabels.data());

        const double startTime = DateUtils::timePointToUnix(data.bedtime);
        const double endTime = DateUtils::timePointToUnix(data.wakeTime);

        //todo тоже хардкод, но эти значения не изменятся
        ImPlot::SetupAxesLimits(startTime, endTime, 0.0, 4.5);
        ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, 4.5);

        ImPlot::SetupAxisFormat(ImAxis_X1, [](double value, char *buff, int size, void *user_data) -> int {
            auto tp = DateUtils::unixToTimePoint(value);
            std::string formatted = DateUtils::onlyTime(tp);
            int written = snprintf(buff, size, "%s", formatted.c_str());
            return (written < 0) ? 0 : written;
        });

        std::vector<double> xTicks;
        xTicks.push_back(DateUtils::timePointToUnix(data.phases.front().start));
        for (const auto &phase: data.phases) {
            const DateTime currentPhaseEnd = phase.end;
            double xEnd = DateUtils::timePointToUnix(currentPhaseEnd);
            xTicks.push_back(xEnd);
        }

        ImPlot::SetupAxisTicks(ImAxis_X1, xTicks.data(), static_cast<int>(xTicks.size()), nullptr);

        for (const auto &phase: data.phases) {
            //todo не переводить два раза в юникс
            const double xStart = DateUtils::timePointToUnix(phase.start);
            const double xEnd = DateUtils::timePointToUnix(phase.end);
            const auto &info = PhaseVisualInfo::getPhaseInfo(phase.type);

            const double xs[2] = {xStart, xEnd};
            const double ys[2] = {info.yLevel, info.yLevel};

            ImPlot::PushStyleColor(ImPlotCol_Line, info.color);
            ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 10.0f);
            ImPlot::PlotLine(info.name, xs, ys, 2);
            ImPlot::PopStyleVar();
            ImPlot::PopStyleColor();
        }

        ImPlot::EndPlot();
    }
    ImGui::End();
}

void Visualization::ShowMetricsSummary(const SleepMetrics &m, const bool isAverage) {
    std::array<PhaseDurationInfo, 3> phases = {{{"Light", m.lightSleepDuration, m.lightSleepPercent},
                                                {"Deep", m.deepSleepDuration, m.deepSleepPercent},
                                                {"REM", m.remSleepDuration, m.remSleepPercent}}};

    ImVec2 windowSize = {ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - 330};
    ImGui::SetNextWindowPos({0, 330}, ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    //todo кастомить строки в другом месте
    ImGui::Begin(isAverage ? "Статистика за неделю" : "Статистика за день", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text(isAverage ? "Cредняя длительность фаз сна (время и проценты):"
                          : "Длительность фаз сна (время и проценты):");
    ImGui::Separator();

    ImGui::Columns(3, "", false);
    if (ImGui::BeginTable("DurationsTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {

        ImGui::TableSetupColumn("Фаза");
        ImGui::TableSetupColumn(isAverage ? "Средняя длительность" : "Длительность");
        ImGui::TableSetupColumn(isAverage ? "Cредняя доля" : "Доля");
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

    ImGui::Text("Время в постели: %s", DateUtils::formatTimeDiff(m.timeInBed).c_str());
    ImGui::Text("Общее время сна: %s", DateUtils::formatTimeDiff(m.totalSleepTime).c_str());
    ImGui::Text("Количество пробуждений: %d", m.awakeningsCount);

    ImGui::NextColumn();
    if (ImPlot::BeginPlot("Доля каждой фазы в минутах", ImVec2(-1, -1), ImPlotFlags_NoLegend)) {
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

        ImPlot::SetupAxisTicks(ImAxis_X1, xIndices.data(), static_cast<int>(xIndices.size()), xLabels.data());

        ImPlot::PlotBars("Фазы", xIndices.data(), yMinutes.data(), static_cast<int>(phases.size()), 0.5);
        ImPlot::EndPlot();
    }

    ImGui::NextColumn();
    if (ImPlot::BeginPlot("Доля каждой фазы на круговой диаграмме", ImVec2(-1, -1),
                          ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs)) {

        ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1);
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations,
                          ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);

        std::vector<double> percentages(phases.size());
        std::vector<const char *> labels(phases.size());

        for (int i = 0; i < phases.size(); i++) {
            percentages[i] = phases[i].percentage;
            labels[i] = phases[i].name;
        }

        ImPlot::PushColormap(ImPlotColormap_Pastel);
        ImPlot::PlotPieChart(labels.data(), percentages.data(), static_cast<int>(phases.size()),
                             0.5, 0.5, 0.4, "%.1f %%", 90.0);
        ImPlot::PopColormap();
        ImPlot::EndPlot();
    }

    ImGui::End();

}

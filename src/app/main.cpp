#include "imgui.h"
#include "implot.h"
#include "GLFW/glfw3.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <string>
#include <iostream>

#include "../sleep_data_loader/DataLoader.h"
#include "SleepAnalyzer.h"
#include "SleepRecommender.h"
#include "Visualization.h"

/**
 * @file
 * @brief Инициализация и запуск приложения для визуализации данных о сне.
 *
 * Этот файл содержит функции для настройки окна GLFW и интеграции с ImGui для
 * рендеринга графического интерфейса. Приложение загружает данные о сне, анализирует
 * их и представляет результаты в виде графического интерфейса.
 */

static const char *glsl_version = "#version 410";

static void glfw_error_callback(int error, const char *description) {
    std::cerr << "GLFW error " << error << ": " << description << std::endl;
}


GLFWwindow *initWindow() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "error: unable to init GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "Sleep Visualizer", nullptr, nullptr);
    if (!window) {
        std::cerr << "error: unable to create window GLFW" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // вкл vsync
    return window;

}

void applyStyle() {
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.GrabRounding = 8.0f;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.96f, 0.98f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.86f, 0.90f, 0.94f, 1.0f);

    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.70f, 0.79f, 0.88f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.70f, 0.79f, 0.88f, 1.0f);

    style.Colors[ImGuiCol_Header] = ImVec4(0.80f, 0.86f, 0.92f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.81f, 0.89f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.68f, 0.75f, 0.84f, 1.0f);

    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.85f, 0.89f, 0.94f, 1.00f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.70f, 0.77f, 0.85f, 1.00f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.82f, 0.86f, 0.91f, 1.00f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.95f, 0.97f, 0.99f, 1.00f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.86f, 0.90f, 0.94f, 1.0f);
}

void initGui(GLFWwindow *window) {

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGui::StyleColorsLight();
    ImPlot::StyleColorsLight();

    applyStyle();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void disposeGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void disposeWindow(GLFWwindow *window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void loadCyrillicFont() {
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("../font/Roboto-Regular.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    ImGui_ImplOpenGL3_CreateFontsTexture();
}

/**
 * @brief Точка входа в программу.
 *
 * @details Загружает данные о сне и запускает главный цикл рендера приложения.
 */
int main() {

    GLFWwindow *window = initWindow();
    initGui(window);

    loadCyrillicFont();

    std::string filePath = {
            "../data/example_data_week.json"
    };
    WeeklySleepData weeklyData = DataLoader::loadFromJsonFile(filePath);
    const DailySleepData todayData = weeklyData.sleepDays[0];

    const SleepMetrics todayMetrics = SleepAnalyzer::CalculateDailyMetrics(todayData);
    const SleepMetrics weeklyMetrics = SleepAnalyzer::CalculateAverageMetrics(weeklyData);

    std::string recommendation = SleepRecommender::GenerateRecommendation(todayMetrics);

    //основной цикл рендера
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Визуализация", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBringToFrontOnFocus);

        if (ImGui::BeginTabBar("MainTabs")) {
            if (ImGui::BeginTabItem("Сегодня")) {
                Visualization::ShowDailyPhasesPlot(todayData);
                Visualization::ShowMetricsSummary(todayMetrics, false);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Неделя")) {
                Visualization::ShowMetricsSummary(weeklyMetrics, true);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    disposeGui();
    disposeWindow(window);

    return 0;
}

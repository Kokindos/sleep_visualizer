cmake_minimum_required(VERSION 3.16)
project(SleepVisualizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(thirdparty)

include_directories(include)


set(SOURCES
        src/main.cpp
        src/DataLoader.cpp
        src/SleepAnalyzer.cpp
        src/SleepRecommender.cpp
        src/Visualization.cpp
        src/DateUtils.cpp
        )


add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME}
        PRIVATE
        nlohmann_json::nlohmann_json
        imgui
        implot
        )

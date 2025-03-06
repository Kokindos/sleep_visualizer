cmake_minimum_required(VERSION 3.16)
project(SleepVisualizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(thirdparty)

include_directories(src/app/include)

set(SOURCES
        src/app/main.cpp
        src/app/SleepAnalyzer.cpp
        src/app/SleepRecommender.cpp
        src/app/Visualization.cpp
        )


add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME}
        PRIVATE
        nlohmann_json::nlohmann_json
        imgui
        implot
        sleep_data_loader
        date_utils
        )

add_subdirectory(src/app/libraries/sleep_data_loader)
add_subdirectory(src/app/libraries/date_utils)
add_subdirectory(src/app/tests)
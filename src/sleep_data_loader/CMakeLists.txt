add_library(sleep_data_loader STATIC DataLoader.h DataLoader.cpp)

target_link_libraries(sleep_data_loader
        PRIVATE
        nlohmann_json::nlohmann_json
        )
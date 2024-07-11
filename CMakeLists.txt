cmake_minimum_required(VERSION 3.20)
project(f1_calendar CXX)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(include)

# Add curl library
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})


# Add vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Vcpkg toolchain file")

# Source files
set(SOURCES
    src/data_service.cpp
    src/ics_writer.cpp
    main.cpp
)

# Executable
add_executable(f1_calendar ${SOURCES})

# Link curl library
target_link_libraries(f1_calendar PRIVATE ${CURL_LIBRARIES})

# Link nlohmann-json
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(f1_calendar PRIVATE nlohmann_json::nlohmann_json)
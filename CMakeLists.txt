cmake_minimum_required(VERSION 3.14)
project(main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")

# --- Pull in cpr automatically ---
include(FetchContent)
FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5)   # pin a version so builds are reproducible
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

# --- Your project's own source files ---
add_executable(main
    src/main.cpp
    src/api.cpp
	src/akari.cpp
	src/cnf.cpp
)

target_include_directories(main PRIVATE include)

target_link_libraries(main PRIVATE cpr::cpr)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

cmake_minimum_required(VERSION 3.21)

project(
    FragTest
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =========================
# Getting FramgentalEngine
# =========================

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../ ${CMAKE_CURRENT_BINARY_DIR}/FragEngineBuild)

# =========================
# Compile Stuff
# =========================

add_executable(FragTest main.cpp)
target_link_libraries(FragTest PRIVATE FragmentalEngine::FragmentalEngine)
cmake_minimum_required(VERSION 3.21)

project(
    FragTest
    VERSION 1.0
    LANGUAGES CXX
)
# =========================
# Compile Stuff
# =========================

add_executable(FragTest main.cpp)

# =========================
# Linking FramgentalEngine
# =========================

target_link_libraries(FragTest PRIVATE FragmentalEngine)
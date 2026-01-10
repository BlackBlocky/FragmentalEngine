cmake_minimum_required(VERSION 3.21)

project(
    FragmentalEngine
    VERSION 0.0.1
    LANGUAGES CXX
)

# =========================
# Global Settings
# =========================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =========================
# Fetching Dependencies
# =========================



# =========================
# Library
# =========================

add_library(FragmentalEngine STATIC)
add_library(FragmentalEngine::FragmentalEngine ALIAS FragmentalEngine)

target_sources(FragmentalEngine
  PRIVATE
    src/Core.cpp
)

target_include_directories(FragmentalEngine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# =========================
# Compiler Warnings
# =========================

if (MSVC)
    target_compile_options(FragmentalEngine PRIVATE /W4 /permissive-)
else()
    target_compile_options(FragmentalEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()
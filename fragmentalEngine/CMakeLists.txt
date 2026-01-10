cmake_minimum_required(VERSION 3.21)

project(
    FragmentalEngine
    VERSION 0.0.1
    LANGUAGES CXX
)

# =========================
# Fetching Dependencies
# =========================

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.10
)

FetchContent_MakeAvailable(glfw)

# =========================
# Library
# =========================

add_library(FragmentalEngine STATIC src/Core.cpp)
target_include_directories(FragmentalEngine
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(FragmentalEngine
  PRIVATE glfw
)
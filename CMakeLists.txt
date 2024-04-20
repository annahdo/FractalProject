cmake_minimum_required(VERSION 3.16)
project(FractalProject VERSION 1.0.0 LANGUAGES CXX)

# Include the toolchain file if CROSS_COMPILE is set to ON
if(CROSS_COMPILE)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/Toolchain-Windows.cmake)
endif()

# Default to release builds
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

find_package(SFML 2 REQUIRED COMPONENTS graphics system window)
find_package(Threads REQUIRED)

set(SFML_LIBS sfml-graphics sfml-system sfml-window)

file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${WIN32_GUI} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")
set(SFML_LIBS sfml-system sfml-window sfml-graphics)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# code for imgui and mgui-sfml

include(FetchContent)

FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.89.9)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(ImGui-SFML)

target_link_libraries(FractalProject sfml-graphics ImGui-SFML::ImGui-SFML pthread)
target_compile_features(FractalProject PRIVATE cxx_std_17)

install(TARGETS FractalProject)

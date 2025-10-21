cmake_minimum_required(VERSION 3.16)
project(MultiplayerTest VERSION 1.0 LANGUAGES C CXX)

# Set CXX standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add source files
file(GLOB SOURCES
    "*.cxx"
    "Scenes/*.cxx"
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(raylib REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

add_subdirectory(external/enet)
target_link_libraries(${PROJECT_NAME} PRIVATE enet)
target_include_directories(${PROJECT_NAME} PRIVATE external/enet/include)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/external/FraxFramework
)

# Optional: enable compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX -g)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -g)
endif()

# Set output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
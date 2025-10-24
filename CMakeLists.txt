cmake_minimum_required(VERSION 3.22)
project(game LANGUAGES CXX)

# --- C++ standard ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Source and include directories ---
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_SOURCE_DIR}/include")

# Grab all .cpp files in src/
file(GLOB_RECURSE SRCS "${SRC_DIR}/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SRCS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIR})

# --- Detect OS and link raylib + dependencies ---
if(APPLE)
    # macOS: Homebrew paths
    find_path(RAYLIB_INCLUDE raylib.h HINTS /opt/homebrew/include /usr/local/include)
    find_library(RAYLIB_LIB raylib HINTS /opt/homebrew/lib /usr/local/lib)

    if(NOT RAYLIB_INCLUDE OR NOT RAYLIB_LIB)
        message(FATAL_ERROR "raylib not found on macOS. Install via Homebrew: brew install raylib")
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYLIB_LIB} m pthread
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )

elseif(UNIX)
    # Linux: standard library names
    find_path(RAYLIB_INCLUDE raylib.h HINTS /usr/include /usr/local/include)
    find_library(RAYLIB_LIB raylib HINTS /usr/lib /usr/local/lib)

    if(NOT RAYLIB_INCLUDE OR NOT RAYLIB_LIB)
        message(FATAL_ERROR "raylib not found on Linux. Install it via your package manager (e.g., sudo apt install libraylib-dev)")
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYLIB_LIB} m dl pthread GL X11)
endif()

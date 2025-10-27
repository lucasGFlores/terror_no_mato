cmake_minimum_required(VERSION 3.22)
project(game LANGUAGES CXX)

# --- C++ standard ---
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
if (APPLE)
    # macOS: Homebrew paths
    find_path(RAYLIB_INCLUDE raylib.h HINTS /opt/homebrew/include /usr/local/include)
    find_library(RAYLIB_LIB raylib HINTS /opt/homebrew/lib /usr/local/lib)

    if (NOT RAYLIB_INCLUDE OR NOT RAYLIB_LIB)
        message(FATAL_ERROR "raylib not found on macOS. Install via Homebrew: brew install raylib")
    endif ()

    target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYLIB_LIB} m pthread
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )

elseif (UNIX)
    # Linux: standard library names
    find_path(RAYLIB_INCLUDE raylib.h HINTS /usr/include /usr/local/include)
    find_library(RAYLIB_LIB raylib HINTS /usr/lib /usr/local/lib)

    if (NOT RAYLIB_INCLUDE OR NOT RAYLIB_LIB)
        message(FATAL_ERROR "raylib not found on Linux. Install it via your package manager (e.g., sudo apt install libraylib-dev)")
    endif ()

    target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYLIB_LIB} m dl pthread GL X11)

elseif (WIN32)
    # Windows
    # Try common installation locations + optional RAYLIB_ROOT
    set(_try_paths
            "${RAYLIB_ROOT}"
            "C:/raylib/w64devkit/x86_64-w64-mingw32"
            "C:/raylib"
            "C:/raylib/raylib/src"
            "C:/msys64/mingw64"
            "C:/Program Files/raylib"
            "C:/Program Files (x86)/raylib"
    )

    # Search for include dir (containing raylib.h) and library file (raylib.lib / libraylib.a)
    find_path(RAYLIB_INCLUDE raylib.h PATHS ${_try_paths} PATH_SUFFIXES include include/raylib NO_DEFAULT_PATH)
    find_path(RAYMATH_INCLUDE raymath.h PATHS ${_try_paths} PATH_SUFFIXES include include/raymath NO_DEFAULT_PATH)
    find_library(RAYLIB_LIB NAMES raylib raylib_static PATHS ${_try_paths} PATH_SUFFIXES lib lib64 lib/raylib NO_DEFAULT_PATH)

    message(STATUS "RAYLIB_INCLUDE = ${RAYLIB_INCLUDE}")
    message(STATUS "RAYMATH_INCLUDE = ${RAYMATH_INCLUDE}")
    message(STATUS "RAYLIB_LIB = ${RAYLIB_LIB}")

    if (NOT RAYLIB_INCLUDE OR NOT RAYLIB_LIB)
        message(FATAL_ERROR "raylib not found on Windows. Set -DRAYLIB_ROOT to the installation folder or install raylib. Searched: ${_try_paths}")
    endif ()

    target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE})
    if (RAYMATH_INCLUDE)
        target_include_directories(${PROJECT_NAME} PRIVATE ${RAYMATH_INCLUDE})
    endif ()

    # Link raylib and typical Windows libs (adjust if using static vs dynamic)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYLIB_LIB} winmm gdi32 opengl32 kernel32 user32)
endif ()

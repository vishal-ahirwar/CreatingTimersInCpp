
#Auto Genrated CMake file by aura CLI
#None
cmake_minimum_required(VERSION 3.6...3.31)
project(timer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(STATIC_LINK "Enable static linking" ON)

# Apply static linking if enabled
if(STATIC_LINK)
  message(STATUS "Static linking enabled")
  # Ensure static runtime linking on Windows
  if (WIN32)
      set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  else()
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
  endif()
endif()

set(COMPANY "None")
string(TIMESTAMP CURRENT_YEAR "%Y")
set(COPYRIGHT "Copyright(c) ${CURRENT_YEAR} ${COMPANY}.")

include_directories(src ${CMAKE_BINARY_DIR})
configure_file(res/config.h.in timerconfig.h)

#@find Warning: Do not remove this line

file(GLOB SOURCES "src/*.cc" "src/*/*.cc")

add_executable(${PROJECT_NAME} ${SOURCES})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Enabling secure coding features for Clang")

    if (MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE
            /W4                      # Enable high warning level
            /WX                      # Treat warnings as errors
            /GS                      # Enable buffer security checks
            /guard:cf                # Enable Control Flow Guard
            /D_FORTIFY_SOURCE=2      # Enable buffer overflow checks
        )
    else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic        # General warnings
        -Wshadow -Wold-style-cast       # Detect potential issues
        -Wcast-align -Wnull-dereference # Runtime safety
        -Wformat=2 -Wformat-security    # Secure formatting
        -fstack-protector-strong        # Stack protection
        -D_FORTIFY_SOURCE=2             # Buffer security
        -fno-common                     # Avoid common symbol issues
        -Werror                         # Treat warnings as errors
    )
    endif()
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
#@link Warning: Do not remove this line


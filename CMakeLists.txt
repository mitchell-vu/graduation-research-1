cmake_minimum_required(VERSION 3.00)
project(gr1 LANGUAGES C)

# Create HEADER-ONLY library
add_library(HEADER-ONLY INTERFACE)

# Add directory "inc/" to target library
add_subdirectory(lib)

# Create a binary executable
add_executable(main main.c)

# Link shared library to main
target_link_libraries(main PRIVATE graph)
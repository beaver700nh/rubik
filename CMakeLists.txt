cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME "rubik")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-Wall -Wextra")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

project(${PROJECT_NAME})

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(
  ${PROJECT_NAME}
  src/main.c
  src/rubik.c
  src/calc.c
)

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES} m)

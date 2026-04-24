cmake_minimum_required(VERSION 3.10)
project(problem_025)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")

add_executable(cheat cheat.cpp lang.cpp)
add_executable(anticheat anticheat.cpp lang.cpp)

# Create code as a copy of cheat for OJ submission
add_custom_command(TARGET cheat POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cheat> ${CMAKE_BINARY_DIR}/code
    COMMENT "Creating code executable from cheat"
)

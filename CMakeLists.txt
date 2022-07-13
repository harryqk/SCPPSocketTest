cmake_minimum_required(VERSION 3.22)
project(SCPPSocketTest)

set(CMAKE_CXX_STANDARD 14)
include_directories(${PROJECT_SOURCE_DIR}/Includes)
link_directories(${PROJECT_SOURCE_DIR}/Lib)

add_executable(SCPPSocketTest main.cpp)



target_link_libraries(SCPPSocketTest libSCPPSocket.a)
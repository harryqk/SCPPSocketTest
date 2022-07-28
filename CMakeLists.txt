cmake_minimum_required(VERSION 3.22)
project(SCPPSocketTest)

set(CMAKE_CXX_STANDARD 14)
if(CMAKE_HOST_WIN32)
    set(WINDOWS 1)
elseif(CMAKE_HOST_APPLE)
    set(MACOS 1)
elseif(CMAKE_HOST_UNIX)
    set(LINUX 1)
endif()


include_directories(${PROJECT_SOURCE_DIR}/Includes)
link_directories(${PROJECT_SOURCE_DIR}/Lib)

add_executable(SCPPSocketTestClient main.cpp)
add_executable(SCPPSocketTestServer main.cpp)
if(MACOS)

    target_link_libraries(SCPPSocketTestClient libSCPPSocketMac.a)
    target_link_libraries(SCPPSocketTestServer libSCPPSocketMac.a)
elseif(WINDOWS)
    target_link_libraries(SCPPSocketTest libSCPPSocketWin.a)
elseif(LINUX)
    target_link_libraries(SCPPSocketTest libSCPPSocketLinux.a)
endif()

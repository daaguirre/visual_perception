cmake_minimum_required(VERSION 3.0.0)
project(VisualPerception VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
add_subdirectory(src)
add_subdirectory(tests)

# APPS
SET(appName robotics_perception)
SET(${appName}_sources
        main.cpp
)
add_executable(${appName} ${${appName}_sources})

target_link_libraries(${appName} 
    VisualPerception
)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

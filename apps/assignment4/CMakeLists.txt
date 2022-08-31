# APPS
SET(appName assignment4)
SET(${appName}_sources
        main.cpp
)

generate_cpp_sources(FILES ${${appName}_sources})

add_executable(${appName} ${${appName}_sources})

target_link_libraries(${appName} 
    VisualPerception
)


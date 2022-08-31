include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_definitions(-DRESOURCES_DIR_PATH="${CMAKE_CURRENT_LIST_DIR}/resources")

SET(appName VisualPerceptionTests)

SET(${appName}_sources
    test_base.h
    test_base.cpp
    vp/fundamental_matrix_estimator_test.h
    vp/fundamental_matrix_estimator_test.cpp
    vp/essential_matrix_estimator_test.h
    vp/essential_matrix_estimator_test.cpp
    vp/point_triangulator_test.h
    vp/point_triangulator_test.cpp
    vp/pnp_algorithm_test.h
    vp/pnp_algorithm_test.cpp
)

generate_cpp_sources(FILES ${${appName}_sources})

add_executable(${appName} ${${appName}_sources})

target_include_directories(${appName} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
) 

target_link_libraries(${appName}
  GTest::gtest_main
  VisualPerception
)

add_test(visual_perception_gtest ${appName})


include(GoogleTest)
gtest_discover_tests(${appName})

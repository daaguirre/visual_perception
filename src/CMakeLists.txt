set(libName VisualPerception)

set(${libName}_sources
        io/bin/vector_io.h
        io/numpy/array.h
        io/numpy/array_io.h
        io/numpy/array_types.h
        io/numpy/npz_reader.h
        io/numpy/npz_reader.cpp
        io/numpy/reader.h
        io/numpy/reader.cpp
        io/numpy/str_array.h
        vp/types.h
        vp/types.cpp
        vp/utils/utils.h
        vp/fundamental_matrix_estimator.h
        vp/fundamental_matrix_estimator.cpp
        vp/essential_matrix_estimator.h
        vp/essential_matrix_estimator.cpp
        vp/point_triangulator.h
        vp/point_triangulator.cpp
        vp/pnp_algorithm.h
        vp/pnp_algorithm.cpp
)

generate_cpp_sources(${${libName}_sources})

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_library(${libName} ${${libName}_sources})

target_include_directories(${libName} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
) 

target_link_libraries(${libName} 
    Eigen3::Eigen
)

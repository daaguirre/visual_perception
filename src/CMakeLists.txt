set(libName VisualPerception)

set(${libName}_sources
        Types.h
        io/bin/VectorIO.h
        io/numpy/Array.h
        io/numpy/ArraySerializer.h
        io/numpy/ArrayTypes.h
        io/numpy/NPZReader.h
        io/numpy/NPZReader.cpp
        io/numpy/Reader.h
        io/numpy/Reader.cpp
        io/numpy/StrArray.h
        utils/utils.h
        fundamental_matrix_estimator.h
        fundamental_matrix_estimator.cpp
)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_library(${libName} ${${libName}_sources})

target_link_libraries(${libName} 
    Eigen3::Eigen
)

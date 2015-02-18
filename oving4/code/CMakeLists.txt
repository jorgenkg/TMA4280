cmake_minimum_required(VERSION 2.6)
project(oving4)

# Include Files , you can add source .cpp and .h to the program
add_executable(summation summation.cpp)
add_executable(summation_mpi summation_mpi.cpp)
add_executable(summation_openmp summation_openmp.cpp)

# Setting up OUTPUT FOLDER as /bin
set(dir ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)


# link MPI
find_package(MPI REQUIRED)
include_directories( ${MPI_C_INCLUDE_PATH} )

target_link_libraries( summation ${MPI_C_LIBRARIES} )
target_link_libraries( summation_mpi ${MPI_C_LIBRARIES} )
target_link_libraries( summation_openmp ${MPI_C_LIBRARIES} )

# include OpenMP
FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
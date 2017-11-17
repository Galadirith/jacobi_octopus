project                     (jacobi_octopus)
cmake_minimum_required      (VERSION 3.0)

# Use package finders
set                         (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# C++11
set                         (CMAKE_CXX_STANDARD        11)
set                         (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# MPI
find_package              (MPI REQUIRED)
if                        (NOT bgq)
  set                     (transport_libraries    ${transport_libraries} ${MPI_C_LIBRARIES} ${MPI_CXX_LIBRARIES})
endif                     ()
add_definitions           (-DTRANSPORT_MPI)
set                       (TRANSPORT_MPI ON)

# CCI
find_package              (CCI REQUIRED)
set                       (transport_libraries    ${transport_libraries} ${MPI_C_LIBRARIES} ${MPI_CXX_LIBRARIES} ${CCI_LIBRARY})
include_directories       (${CCI_INCLUDE_DIR})
add_definitions           (-DTRANSPORT_CCI)
set                       (TRANSPORT_CCI ON)

# File
set                       (TRANSPORT_FILE OFF)

# Boost
find_package                (Boost 1.59.0 COMPONENTS serialization REQUIRED)
message                     (STATUS "Boost libraries: " ${Boost_LIBRARIES})

# Decaf
find_package              (Decaf REQUIRED)
include_directories       (${DECAF_CXX_INCLUDE_DIR} ${DECAF_C_INCLUDE_DIR})

# Set include directories
set                         (CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-isystem")
include_directories         (${Boost_INCLUDE_DIRS}
                             SYSTEM ${MPI_INCLUDE_PATH})

# Set libraries
set                         (libraries
                             ${libraries}
                             ${transport_libraries}
                             ${CMAKE_DL_LIBS}
                             ${DECAF_CXX_DATA_MODEL_LIBRARY}
                             ${DECAF_CXX_TRANSPORT_MPI_LIBRARY}
                             ${DECAF_CXX_TRANSPORT_CCI_LIBRARY}
                             ${DECAF_C_DATA_MODEL_LIBRARY}
                             ${DECAF_C_RUNTIME_LIBRARY}
                             ${DECAF_CXX_RUNTIME_LIBRARY}
                             ${MANALA_LIBRARY}
                             ${Boost_LIBRARIES})

add_executable              (jacobi                  jacobi.cpp)
add_executable              (jacobi_analyse          jacobi_analyse.cpp)
target_link_libraries       (jacobi ${libraries})
target_link_libraries       (jacobi_analyse ${libraries})

cmake_minimum_required(VERSION 3.9)
project( teensy_server CXX )

# set(CMAKE_CXX_FLAGS "-lpthread -pthread")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
# Add -Weffc++ for all sorts of good errors to fix
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")

################################################################################
# GLOBAL SETUP
################################################################################
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Eigen setup
find_package(Eigen3 REQUIRED)
if (NOT Eigen3_FOUND)
message(STATUS "This project requires Eigen3 and will not be compiled.")
return()
endif()
include(${EIGEN3_USE_FILE})

################################################################################
# BUILD TARGETS
################################################################################
include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(main src/main.cpp)
# target_link_libraries(main

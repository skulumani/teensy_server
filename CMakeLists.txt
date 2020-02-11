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

# libserialport
find_package (PkgConfig REQUIRED)

# link_directories("/usr/local/lib/")
# include_directories("/usr/local/include/")
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

pkg_check_modules(LIBSERIALPORT libserialport)
link_directories(${LIBSERIALPORT_LIBRARY_DIRS})
include_directories(${LIBSERIALPORT_INCLUDE_DIRS})

# include(FindProtobuf)
find_package(Protobuf REQUIRED)
# add_subdirectory(${PROJECT_SOURCE_DIR}/proto)
# include_directories(${CMAKE_CURRENT_BINARY_DIR})

# include_directories(${Protobuf_INCLUDE_DIRS})
protobuf_generate_cpp(PROTO_SIMPLE_SRCS PROTO_SIMPLE_HDRS ${PROJECT_SOURCE_DIR}/proto/simple.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROJECT_SOURCE_DIR}/proto/ahrs.proto)

message("PROTO_SRCS = ${PROTO_SRCS}")
message("PROTO_HDRS = ${PROTO_HDRS}")

# add_library(protobuf ${PROTO_HDRS} ${PROTO_SRC})
# protobuf_generate_python(PROTO_PY foo.proto)
# add_library(protobuf ${PROTO_HDRS} ${PROTO_SRC})

# message(status, "Subdir Protobuf lib is ${PROTOBUF_LIBRARY}")
# message(status, "Subdir Protobuf libs is ${PROTOBUF_LIBRARIES}")
################################################################################
# BUILD TARGETS
################################################################################
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(read_serial_example src/read_serial_example.cpp)
target_link_libraries(read_serial_example PUBLIC serialport)

add_executable(main 
    src/main.cpp
    src/serial_port.cpp
    ${PROTO_SRCS} 
    ${PROTO_HDRS}
    ${PROTO_SIMPLE_SRCS}
    ${PROTO_SIMPLE_HDRS})
target_link_libraries(main PUBLIC serialport)
target_link_libraries(main PUBLIC ${Protobuf_LIBRARIES})
target_include_directories(main
    PUBLIC
    ${PROTOBUF_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR})

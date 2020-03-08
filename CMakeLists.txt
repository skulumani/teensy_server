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

# GLFW
find_package(glfw3 3.3 QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW3 not found")
else()
    message(STATUS "GLFW3 found. ")
endif()

# Vulkan
find_package(Vulkan QUIET)
if(NOT Vulkan_FOUND)
    message(STATUS "Vulkan not found")
else()
    message(STATUS "Vulkan found. ")
endif()

# FDCL-HDF5 for storing data
find_package(FDCL_HDF5 REQUIRED)
if(NOT FDCL_HDF5_FOUND)
    message(STATUS "FDCL HDF5 not found")
else()
    message(STATUS "FDCL HDF5 found. ")
endif()

find_package(HDF5 REQUIRED COMPONENTS CXX)
if (NOT HDF5_FOUND)
    message(STATUS "This project requires HDF5 and will not compile")
    return()
else()
    message(STATUS "Found HDF5: ${HDF5_VERSION}")
endif()

if ( HDF5_IS_PARALLEL )
    message(STATUS "HDF5 parallel is installed")
else()
    message(STATUS "HDF5 parallel is NOT installed")
endif()
# message(STATUS "HDF5 Lib: ${HDF5_LIBRARIES}")
include_directories(${HDF5_INCLUDE_DIRS})
set(hdf5_libs hdf5 hdf5_cpp)
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
target_link_libraries(main PUBLIC fdcl_hdf5)

if (Vulkan_FOUND AND glfw3_FOUND)
    add_executable(vulkan_example src/vulkan_example.cpp)
    target_link_libraries(vulkan_example glfw)
    target_link_libraries(vulkan_example Vulkan::Vulkan)
    target_include_directories(vulkan_example PRIVATE Vulkan::Vulkan)

    add_executable(hello_triangle src/hello_triangle.cpp)
    target_link_libraries(hello_triangle glfw)
    target_link_libraries(hello_triangle Vulkan::Vulkan)
    target_include_directories(hello_triangle PRIVATE Vulkan::Vulkan)
endif()

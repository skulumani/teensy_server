# include(FindProtobuf)
# find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER simple.proto)
add_library(protobuf ${PROTO_HEADER} ${PROTO_SRC})

message(status, "Subdir Protobuf lib is ${PROTOBUF_LIBRARY}")
message(status, "Subdir Protobuf libs is ${PROTOBUF_LIBRARIES}")

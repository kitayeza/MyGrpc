
cmake_minimum_required(VERSION 3.12)
project(grpc-my-project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED PATHS /home/yzhen/repo/.local/lib)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(PROTO_PATH "${CMAKE_SOURCE_DIR}/protos")
set(HELLOWORLD_PROTO "${PROTO_PATH}/receiver.proto")
set(GENERATED_PROTOBUF_PATH "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_PROTOBUF_PATH})

set(HELLOWORLD_PB_CPP_FILE "${GENERATED_PROTOBUF_PATH}/receiver.pb.cc")
set(HELLOWORLD_PB_H_FILE "${GENERATED_PROTOBUF_PATH}/receiver.pb.h")
set(HELLOWORLD_GRPC_PB_CPP_FILE "${GENERATED_PROTOBUF_PATH}/receiver.grpc.pb.cc")
set(HELLOWORLD_GRPC_PB_H_FILE "${GENERATED_PROTOBUF_PATH}/receiver.grpc.pb.h")

add_custom_command(
        OUTPUT "${HELLOWORLD_PB_H_FILE}"
        "${HELLOWORLD_PB_CPP_FILE}"
        "${HELLOWORLD_GRPC_PB_H_FILE}"
        "${HELLOWORLD_GRPC_PB_CPP_FILE}"
        COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
        ARGS "--proto_path=${PROTO_PATH}"
        "--cpp_out=${GENERATED_PROTOBUF_PATH}"
        "${HELLOWORLD_PROTO}"
        COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
        ARGS "--proto_path=${PROTO_PATH}"
        "--grpc_out=${GENERATED_PROTOBUF_PATH}"
        "--plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin"
        "${HELLOWORLD_PROTO}"
)
set(GENERATED_PROTOBUF_FILES ${HELLOWORLD_PB_H_FILE} ${HELLOWORLD_PB_CPP_FILE}
        ${HELLOWORLD_GRPC_PB_H_FILE} ${HELLOWORLD_GRPC_PB_CPP_FILE})

include_directories(${GENERATED_PROTOBUF_PATH})

add_executable(gdata_server gdata_server.cc ${GENERATED_PROTOBUF_FILES})
add_executable(gdata_client gdata_client.cc ${GENERATED_PROTOBUF_FILES})
target_link_libraries(gdata_server gRPC::grpc++)
target_link_libraries(gdata_client gRPC::grpc++)


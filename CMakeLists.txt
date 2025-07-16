# Set the minimum CMake version.
cmake_minimum_required(VERSION 3.20.0)

# Set the project name and the language.
set(PROJECT_NAME brpc-echo)
project(${PROJECT_NAME} LANGUAGES CXX)

# Set the C++ version to C++23.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output folders.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Find Protocol Buffers.
find_package(Protobuf REQUIRED)

# Find bRPC.
# Set the path of bRPC output directory.
find_path(BRPC_INCLUDE_DIR brpc/server.h PATHS ${BRPC_INSTALL_DIR}/include)
find_library(BRPC_LIB NAMES brpc PATHS ${BRPC_INSTALL_DIR}/lib)

if ((NOT BRPC_INCLUDE_DIR) OR (NOT BRPC_LIB))
    message(FATAL_ERROR "Failed to find bRPC")
endif()

# Define the root directory of `.proto` files.
set(PROTO_ROOT_DIR ${PROJECT_SOURCE_DIR}/proto)
# Define the output directory for generated `.pb.h` and `.pb.cc` files
set(PROTO_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
# Make sure the generated directory exists.
file(MAKE_DIRECTORY ${PROTO_GENERATED_DIR})

# Ignore deprecated function or variable declarations.
add_compile_options(-Wno-deprecated-declarations)

######################################################################

# Set the name of the common Protocol Buffers types.
set(COMMON_PROTO_NAME msg_proto)

# Create a library target for the generated Protocol Buffers library.
add_library(${COMMON_PROTO_NAME})

# Add `.proto` files.
set(COMMON_PROTO_FILES ${PROTO_ROOT_DIR}/msg.proto)

# # Generate C++ source and header files from `.proto` files.
protobuf_generate(
    TARGET ${COMMON_PROTO_NAME}
    PROTOC_OUT_DIR ${PROTO_GENERATED_DIR}
    PROTOS ${COMMON_PROTO_FILES}
    IMPORT_DIRS ${PROTO_ROOT_DIR}
)

target_include_directories(${COMMON_PROTO_NAME}
    PUBLIC
        ${PROTO_GENERATED_DIR}
        ${Protobuf_INCLUDE_DIRS}
)

target_link_libraries(${COMMON_PROTO_NAME}
    PRIVATE
        protobuf::libprotobuf
)

######################################################################

# Set the name of the Protocol Buffers service.
set(PROTO_SERVICE_NAME echo_proto)

# Create a library target for the generated Protocol Buffers service.
add_library(${PROTO_SERVICE_NAME})

# Add `.proto` files.
set(PROTO_SERVICE_FILES ${PROTO_ROOT_DIR}/echo.proto)

# # Generate C++ source and header files from `.proto` files.
protobuf_generate(
    TARGET ${PROTO_SERVICE_NAME}
    PROTOC_OUT_DIR ${PROTO_GENERATED_DIR}
    PROTOS ${PROTO_SERVICE_FILES}
    IMPORT_DIRS ${PROTO_ROOT_DIR}
)

target_include_directories(${PROTO_SERVICE_NAME}
    PUBLIC
        ${PROTO_GENERATED_DIR}
        ${BRPC_INCLUDE_DIR}
        ${Protobuf_INCLUDE_DIRS}
)

target_link_libraries(${PROTO_SERVICE_NAME}
    PRIVATE
        protobuf::libprotobuf
        ${BRPC_LIB}
        ${COMMON_PROTO_NAME}
)

######################################################################

# Process source files.
add_subdirectory(server)
add_subdirectory(client)
cmake_minimum_required(VERSION 3.15)
project(xios)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(APPLE)
    set(WOLFSSL_ARCHIVE "build-osx-arm64.tar.gz")
elseif(UNIX)
    set(WOLFSSL_ARCHIVE "build-x86_64.tar.gz")
endif()

include(FetchContent)

# ---- Fetch Catch2 (for unit tests) ----
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# ---- Add xios library ----
file(GLOB_RECURSE XIOS_SRC
    src/*.cpp
    secure_http_client_napi.cpp
)

# Attempt to auto-detect node_api.h path from NVM
file(GLOB NODE_INCLUDE_DIR "$ENV{HOME}/.nvm/versions/node/*/include/node")

if(NODE_INCLUDE_DIR)
    message(STATUS "✅ Found Node.js include dir: ${NODE_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "❌ node_api.h not found.")
endif()

add_library(xios STATIC ${XIOS_SRC})

target_include_directories(xios
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/${WOLFSSL_ARCHIVE}
        ${PROJECT_SOURCE_DIR}/node_modules/node-addon-api
        ${NODE_INCLUDE_DIR}
)

target_link_directories(xios
    PUBLIC
        ${PROJECT_SOURCE_DIR}/lib/
)

target_link_libraries(xios
    PUBLIC
        wolfssl
        sqlite3
        pthread
)
add_custom_command(TARGET xios POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/lib/
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/libwolfssl.a ${CMAKE_BINARY_DIR}/lib/libwolfssl.a
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/sqlite3.o ${CMAKE_BINARY_DIR}/lib/sqlite3.o
    COMMAND cd ${CMAKE_BINARY_DIR}/lib/ && ar x ${CMAKE_BINARY_DIR}/lib/libwolfssl.a
    COMMAND ar rvs ${CMAKE_BINARY_DIR}/lib/libxios.a ${CMAKE_BINARY_DIR}/lib/*.o
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Rebuilding static xios with wolfSSL objects"
)

add_custom_command(TARGET xios POST_BUILD
    COMMAND rm -f ${CMAKE_SOURCE_DIR}/lib/libxios.a
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/lib/libxios.a ${CMAKE_SOURCE_DIR}/lib/libxios.a
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Copying libxios.a to lib directory"
)

# ---- Add xios_tests ----
add_executable(xios_tests
    test/test_main.cpp
    test/test_get.cpp
    test/test_post.cpp
    test/test_parseURL.cpp
)

target_include_directories(xios_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/test
        ${PROJECT_SOURCE_DIR}/test/catch2/include
)

target_link_libraries(xios_tests
    PRIVATE
        xios
        Catch2::Catch2WithMain
)

# ---- Enable CTest & Catch2 autodiscovery ----
include(CTest)
include(Catch)
catch_discover_tests(xios_tests)
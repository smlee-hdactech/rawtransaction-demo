cmake_minimum_required(VERSION 3.1)

project(ass-send)

include(FindBoost)
include(FindOpenSSL)
include(FindPkgConfig)

find_package(Boost 1.55.0 REQUIRED system thread)
find_package(OpenSSL REQUIRED)
pkg_check_modules (SECP256K1 REQUIRED libsecp256k1)
pkg_check_modules (HS_HELPERS REQUIRED hs_helpers)
pkg_check_modules (HS_KEYS REQUIRED hs_keys)
pkg_check_modules (HS_SCRIPT REQUIRED hs_script)
pkg_check_modules (HS_STRUCTS REQUIRED hs_structs)
pkg_check_modules (HS_RPC REQUIRED hs_rpc)
pkg_check_modules (HS_UTILS REQUIRED hs_utils)
pkg_check_modules (HS_CRYPTO REQUIRED hs_crypto)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${HS_HELPERS_INCLUDE_DIRS}
    ${HS_KEYS_INCLUDE_DIRS}
    ${HS_SCRIPT_INCLUDE_DIRS}
    ${HS_STRUCTS_INCLUDE_DIRS}
    ${HS_RPC_INCLUDE_DIRS}
    ${HS_UTILS_INCLUDE_DIRS}
    ${HS_CRYPTO_INCLUDE_DIRS}
    )

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${HS_HELPERS_LIBRARIES}
    ${HS_KEYS_LIBRARIES}
    ${HS_SCRIPT_LIBRARIES}
    ${HS_STRUCTS_LIBRARIES}
    ${HS_RPC_LIBRARIES}
    ${HS_UTILS_LIBRARIES}
    ${HS_CRYPTO_LIBRARIES}
    json_spirit
    ${SECP256K1_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    Boost::thread
    Boost::system    
    )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

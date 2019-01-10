cmake_minimum_required(VERSION 3.1)

project(RAWTX-DEMO)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_DEBUG_POSTFIX "-dbg")

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

# subdirectories
add_subdirectory(ass-send)
add_subdirectory(str-pub)


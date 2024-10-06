cmake_minimum_required(VERSION "3.27")

project(kiramitm)
set(CMAKE_CXX_STANDARD 26)

#file(GLOB_RECURSE SRC_FILES
#        "${CMAKE_SOURCE_DIR}/src/*.cpp"
#        "${CMAKE_SOURCE_DIR}/src/*.c"
#)
#
#add_executable(kiramitm ${SRC_FILES})
#
#find_package(OpenSSL REQUIRED)
#find_package(asio CONFIG REQUIRED)
#find_package(spdlog CONFIG REQUIRED)
#find_package(argparse CONFIG REQUIRED)
#
#if (UNIX)
#    target_link_libraries(kiramitm PRIVATE pthread)
#endif ()
#
#target_link_libraries(kiramitm PRIVATE
#        asio::asio
#        spdlog::spdlog
#        OpenSSL::SSL
#        argparse::argparse
#)

add_subdirectory(kmp-common)
add_subdirectory(kmp-core)
add_subdirectory(samples)
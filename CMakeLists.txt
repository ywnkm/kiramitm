cmake_minimum_required(VERSION "3.27")


project(kiramitm)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")

file(GLOB_RECURSE SRC_FILES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/*.c"
)

add_executable(kiramitm ${SRC_FILES})

find_package(OpenSSL REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_link_libraries(kiramitm PRIVATE
        asio::asio
        spdlog::spdlog
        pthread
        OpenSSL::SSL
)

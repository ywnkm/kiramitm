

file(GLOB_RECURSE SRC_FILES
        "src/*.cpp"
        "src/*.c"
        "include/*.hpp"
        "include/*.h"
)

find_package(OpenSSL REQUIRED)
find_package(asio CONFIG REQUIRED)

add_library(kmp-core STATIC ${SRC_FILES})
target_link_libraries(kmp-core PUBLIC kmp-common)
target_link_libraries(kmp-core PRIVATE asio::asio OpenSSL::SSL)


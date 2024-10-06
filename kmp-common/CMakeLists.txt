
file(GLOB_RECURSE SRC_FILES
        "src/*.cpp"
        "src/*.c"
        "include/*.hpp"
        "include/*.h"
)

find_package(spdlog CONFIG REQUIRED)

add_library(kmp-common STATIC ${SRC_FILES})
target_include_directories(kmp-common PUBLIC include)
target_link_libraries(kmp-common PUBLIC spdlog::spdlog)
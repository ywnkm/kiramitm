
include_directories(
    ${CMAKE_SOURCE_DIR}/kmp-common/include
    ${CMAKE_SOURCE_DIR}/kmp-core/include)

link_libraries(kmp-core kmp-common)

add_subdirectory(server)
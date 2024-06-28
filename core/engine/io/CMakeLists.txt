FILE(GLOB_RECURSE HEADERS "include/**.h*")
FILE(GLOB_RECURSE SRC "src/**.c*")

find_package(stduuid REQUIRED)
find_package(minizip-ng CONFIG REQUIRED)

add_library(IO SHARED ${SRC} ${HEADERS})

target_include_directories(IO PRIVATE include)
target_include_directories(IO PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)
target_include_directories(IO PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/crypto/include)
target_include_directories(IO PRIVATE ${PHYSFS_INCLUDE_DIR})

target_link_libraries(IO PUBLIC MINIZIP::minizip-ng Crypto)

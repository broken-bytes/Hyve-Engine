FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.cxx")

add_library(Crypto SHARED ${SRC} ${HEADERS})

target_include_directories(Crypto PRIVATE include)
target_include_directories(Crypto PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)

find_package(unofficial-sodium CONFIG REQUIRED)

target_link_libraries(Crypto PRIVATE unofficial-sodium::sodium)
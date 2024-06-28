FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.cxx")

add_executable(bundler ${SRC} ${HEADERS})

target_include_directories(bundler PRIVATE include)
target_include_directories(bundler PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/io/include)
target_include_directories(bundler PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/crypto/include)
target_include_directories(bundler PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(minizip-ng CONFIG REQUIRED)

target_link_libraries(bundler IO Crypto MINIZIP::minizip-ng nlohmann_json::nlohmann_json)
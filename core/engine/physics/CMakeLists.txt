FILE(GLOB_RECURSE HEADERS "include/**.h*")
FILE(GLOB_RECURSE SRC "src/**.c*")

add_library(Physics SHARED ${SRC} ${HEADERS})

target_include_directories(Physics PRIVATE include)
target_include_directories(Physics PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)
target_include_directories(Physics PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/logger/include)
target_include_directories(Physics PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/math/include)

find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)

target_link_libraries(Physics PUBLIC Logger)
target_link_libraries(Physics PRIVATE spdlog::spdlog glm::glm box2d::box2d)

find_path(ATOMIC_QUEUE_INCLUDE_DIRS "atomic_queue/atomic_queue.h")
target_include_directories(Physics PRIVATE ${ATOMIC_QUEUE_INCLUDE_DIRS})

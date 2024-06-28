FILE(GLOB_RECURSE HEADERS "include/*.h*")
FILE(GLOB_RECURSE SRC "src/*.cxx")

add_library(Audio SHARED ${SRC} ${HEADERS})

target_include_directories(Audio PRIVATE include)
target_include_directories(Audio PRIVATE ${CMAKE_SOURCE_DIR}/core/engine/shared/include)

find_package(OpenAL CONFIG REQUIRED)
find_package(AudioFile CONFIG REQUIRED)
find_package(FreeALUT CONFIG REQUIRED)
find_package(cereal CONFIG REQUIRED)

target_link_libraries(Audio PRIVATE OpenAL::OpenAL)
target_link_libraries(Audio PRIVATE FreeALUT::alut)
target_link_libraries(Audio PRIVATE AudioFile)

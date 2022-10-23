set(BINARY y86Emulator)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set (SOURCES ${SOURCES})

add_library(lib${BINARY} STATIC ${SOURCES})

add_executable(${BINARY} ${SOURCES})

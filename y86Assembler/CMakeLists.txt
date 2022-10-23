set(BINARY y86Assembler)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})

include_directories(include)

add_executable(${BINARY} ${SOURCES})

add_library(lib${BINARY} STATIC ${SOURCES})
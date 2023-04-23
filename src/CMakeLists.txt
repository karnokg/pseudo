find_package(BISON)
find_package(FLEX)

BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp
            COMPILE_FLAGS "-Wconflicts-sr -Wcounterexamples -t -Dparse.trace"
            DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser.h)
FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_SOURCE_DIR}/lexer.cpp)

ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE PSEUDO_SOURCES
  *.cpp
  *.h
)

add_executable(pseudo
  ${BISON_Parser_OUTPUTS}
  ${FLEX_Lexer_OUTPUTS}
  ${PSEUDO_SOURCES}
)

#target_link_libraries(pseudo
#)
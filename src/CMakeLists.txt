cmake_minimum_required(VERSION 3.0.0)
project(pseudo VERSION 0.0.1 LANGUAGES CXX C)

find_package(BISON)
find_package(FLEX)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# -report=solved
# -Wcounterexamples
BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp
            COMPILE_FLAGS "-Wconflicts-sr -t -Dparse.trace"
            DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser.h)
FLEX_TARGET(Lexer lexer.l ${CMAKE_CURRENT_SOURCE_DIR}/lexer.cpp)

ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

message("asdasdasdasd")
message(${LLVM_INCLUDE_DIRS})

#file(GLOB_RECURSE PSEUDO_SOURCES
#  *.cpp
#  *.h
#)

set(PSEUDO_SOURCES
  externfunctions.cpp
  main.cpp
  CodeGenContext.cpp
  interpreter.cpp
  pseudo.cpp
  visitor.cpp
  visitorprintast.cpp
  visitorsyntaxcheck.cpp
  nodes/node.cpp
  nodes/binaryoperator.cpp
  nodes/functiondeclaration.cpp
  nodes/unaryoperator.cpp
  nodes/variabledeclaration.cpp
  nodes/compareoperator.cpp
  nodes/assignment.cpp
  nodes/string.cpp
  nodes/vector.cpp
  nodes/forloop.cpp
)

set(PSEUDO_HEADERS
  externfunctions.h
  CodeGenContext.h
  interpreter.h
  node.h
  pseudo.h
  scanner.h
  visitor.h
  visitorprintast.h
  visitorsyntaxcheck.h
  nodes/binaryoperator.h
  nodes/functiondeclaration.h
  nodes/unaryoperator.h
  nodes/variabledeclaration.h
  nodes/compareoperator.h
  nodes/assignment.h
  nodes/string.h
  nodes/vector.h
  nodes/forloop.h
)

add_executable(pseudo
  ${BISON_Parser_OUTPUTS}
  ${FLEX_Lexer_OUTPUTS}
  ${PSEUDO_SOURCES}
  ${PSEUDO_HEADERS}
)

set (CMAKE_CXX_FLAGS "-lSegFault")

llvm_map_components_to_libnames(llvm_libs support core irreader mcjit native interpreter ipo)
target_link_libraries(pseudo ${llvm_libs})

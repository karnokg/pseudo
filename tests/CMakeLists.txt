add_executable(PSEUDO_TEST test.cpp)

target_link_libraries(PSEUDO_TEST pseudo)

include(CTest)

add_test(
  NAME
    testPseudoCodes
  COMMAND
    $<TARGET_FILE:PSEUDO_TEST>
)
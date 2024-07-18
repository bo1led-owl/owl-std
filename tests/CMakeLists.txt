find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

macro(add_test_target NAME)
  add_executable(
    ${NAME}
    ${ARGN}
  )
  target_include_directories(${NAME}
    PRIVATE 
      ${PROJECT_SOURCE_DIR}/src/
  )
  target_link_libraries(${NAME}
    PRIVATE
      GTest::gtest_main
      ${PROJECT_NAME}
  )
    
  target_compile_options(${NAME} PRIVATE -fsanitize=address,undefined)
  target_link_options(${NAME} PRIVATE -fsanitize=address,undefined)

  gtest_discover_tests(${NAME})
endmacro()

set(TESTS
  vector
  str
)

set(TESTS_SRC
  vector.cc
  str.cc
)

foreach(test file IN ZIP_LISTS TESTS TESTS_SRC)
  add_test_target(${test} ${file})
endforeach()

add_test_target(full ${TESTS_SRC})

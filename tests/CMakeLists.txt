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

add_test_target(tests
  vector.cc
)

function(add_example NAME SRC)
  add_executable("${NAME}_example" ${SRC})
  set_property(TARGET "${NAME}_example" PROPERTY OUTPUT_NAME ${NAME})
  target_include_directories("${NAME}_example" PRIVATE ${CMAKE_SOURCE_DIR}/include)
  target_link_libraries("${NAME}_example" PRIVATE ${PROJECT_NAME})
endfunction()

add_example(str str.c)
add_example(vector vector.c)

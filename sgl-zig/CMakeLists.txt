project(sgl LANGUAGES NONE)
cmake_minimum_required(VERSION 3.8)

find_program(ZIG zig)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(ZIG_ARGS "-Drelease-safe")
endif()

set(LIB_FILE "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.a")

set(ZIG_CMD ${ZIG} build -Doutput-dir="${CMAKE_CURRENT_BINARY_DIR}" ${ZIG_ARGS})

file(GLOB_RECURSE ZIG_SRC "src/*.zig")

add_custom_command(
  OUTPUT ${LIB_FILE}
  COMMAND ${ZIG_CMD}
  DEPENDS build.zig ${ZIG_SRC}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(${PROJECT_NAME}_target ALL DEPENDS ${LIB_FILE})

add_library(${PROJECT_NAME} STATIC IMPORTED GLOBAL)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_target)

file(GLOB SGL_PUBLIC_HDR "include/*.h")

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  IMPORTED_LOCATION ${LIB_FILE}
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(FILES ${LIB_FILE} TYPE LIB)
set(_RESOLVED_HDR "")
foreach (_FILE ${SGL_PUBLIC_HDR})
  get_filename_component(_RESOLVED "${_FILE}" REALPATH)
  list(APPEND _RESOLVED_HDR "${_RESOLVED}")
endforeach()
install(FILES ${_RESOLVED_HDR} TYPE INCLUDE)

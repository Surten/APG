project(sgl LANGUAGES NONE)
cmake_minimum_required(VERSION 3.8)

find_program(CARGO cargo)

# Default to debug
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  list(APPEND CARGO_ARGS "--release")
  set(TARGET_DIR "release")
else()
  set(TARGET_DIR "debug")
endif()

# Location of the final library
set(LIB_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_DIR}/lib${PROJECT_NAME}.a")

# Cargo command to call
set(RUST_CMD CARGO_TARGET_DIR=${CMAKE_CURRENT_BINARY_DIR} ${CARGO} build ${CARGO_ARGS} --verbose)

# Source files
file(GLOB_RECURSE RUST_SRC "src/*.rs")

# Command object to build the library
add_custom_command(
  # Outputs the library file
  OUTPUT ${LIB_FILE}
  # Using the `cargo` command
  COMMAND ${RUST_CMD}
  # Reruns when the manifest or sources change
  DEPENDS Cargo.toml ${RUST_SRC}
  # Runs in the source directory
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  # And prints output while building
  USES_TERMINAL
)

# Target for building the library, call with `cmake --build build -t sgl_target`
# Necessary because IMPORTED library targets (like the one below) don't have associated rules
add_custom_target(${PROJECT_NAME}_target ALL DEPENDS ${LIB_FILE})

# Proxy library declaration for interaction with CMake
# This makes it so that `sgl` can be used in `target_link_libraries`
add_library(${PROJECT_NAME} STATIC IMPORTED GLOBAL)
# The library needs the custom target
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_target)

# Tell the library users where to look for header files and what to link
set_target_properties(${PROJECT_NAME}
  PROPERTIES
  IMPORTED_LOCATION ${LIB_FILE}
  INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
  INTERFACE_LINK_LIBRARIES "pthread;dl"
)

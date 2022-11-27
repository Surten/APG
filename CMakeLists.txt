# Min. CMake version.
cmake_minimum_required(VERSION 3.8)

# Project name.
set(NAME testapp)

# A name of the project.
project(${NAME})

# GLUT.
find_package(GLUT)

# OpenGL.
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


# Set general compiler flags.
if (WIN32)
  add_definitions(-D__WINDOWS__ -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
endif (WIN32)

if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework OpenGL -framework GLUT")
endif (APPLE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

# Do not use GUI by default.
add_definitions(-DUSE_GUI=0)

set(HDR
  hdrloader.h
  nffread.h
  nffstore.h
  nffwrite.h
  timer.h
)

set(SRC
  hdrloader.cpp
  nffread.cpp
  testapp.cpp
  timer.cpp
)

source_group("Header Files\\" FILES ${HDR})
source_group("Source Files\\" FILES ${SRC})

# Add all files to excutable.
add_executable(${NAME}
  ${HDR}
  ${SRC}
)

# Use freeglut on Windows.
if (WIN32)
  if (NOT GLUT_FOUND)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(GLUT_LIB_PATH freeglut/lib/x64)
  else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(GLUT_LIB_PATH freeglut/lib)
  endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
  find_library(GLUT_LIBRARIES
    NAMES freeglut
    PATHS
    ${GLUT_LIB_PATH}
    DOC "The GLUT library"
  )
  find_path(GLUT_INCLUDE_DIR GL/glut.h
    freeglut/include
    DOC "The directory where GL/glut.h resides"
  )
  endif (NOT GLUT_FOUND)
endif (WIN32)

if(SGL_BACKEND STREQUAL "rust")
  message(STATUS "Using Rust backend")
  add_subdirectory(sgl-rust)
elseif(SGL_BACKEND STREQUAL "zig")
  message(STATUS "Using Zig backend")
  add_subdirectory(sgl-zig)
else()
  message(STATUS "Using C++ backend")
  add_subdirectory(sgl-cpp)
endif()

# Link libraries.
target_link_libraries(${NAME} sgl ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

# Set output directory.
set(BIN_DIR ${CMAKE_SOURCE_DIR})

# Properties.
set_target_properties(${NAME} PROPERTIES
  DEBUG_OUTPUT_NAME ${NAME}_d
  RELEASE_OUTPUT_NAME ${NAME}
  RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}
  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}
)

install(TARGETS ${NAME}
  RUNTIME DESTINATION ${BIN_DIR}
)

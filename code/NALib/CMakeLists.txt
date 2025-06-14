cmake_minimum_required(VERSION 3.13)
project(NALib)

set(TARGET_NAME NALib)
add_library(NALib STATIC)

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE C)
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
  # multi processor compilation
  target_compile_options(${TARGET_NAME} PRIVATE /MP)
else()
endif()

set(primaryHeaderFile
  src/NALib.h
)

# When on Windows, we use MSVC, when on macOS, we use MACOSX as switch.
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(MACOSX TRUE)
endif()

source_group("" FILES ${primaryHeaderFile})
target_sources(${TARGET_NAME} PRIVATE ${primaryHeaderFile})

include(src/NAApp/CMakeSrcList.txt)
include(src/NABase/CMakeSrcList.txt)
include(src/NAMath/CMakeSrcList.txt)
include(src/NAStruct/CMakeSrcList.txt)
include(src/NAUtility/CMakeSrcList.txt)
include(src/NAVisual/CMakeSrcList.txt)



# ############ Configure for GUI support ###################
# Provide the two most common options for the CMake GUI.
OPTION(COMPILE_NALIB_GUI "Compile GUI support" OFF)
OPTION(COMPILE_NALIB_OPENGL "Compile OpenGL support" OFF)
if(COMPILE_NALIB_GUI)
  add_definitions(-DNA_COMPILE_GUI=1)
endif()
if(COMPILE_NALIB_OPENGL)
  add_definitions(-DNA_COMPILE_OPENGL=1)
endif()



# ############ Enable testing ###################
if(${ENABLE_NALIB_TESTING})
  add_definitions(-DNA_TESTING_ENABLED=1)
  set(NATEST_DIR "../../lib/NATest/code/NATest")
  target_include_directories(${TARGET_NAME} PRIVATE ${NATEST_DIR}/src)
  if(NOT TARGET NATest)
    add_subdirectory(${NATEST_DIR} NATest)
   endif()
  target_link_libraries(${TARGET_NAME} PRIVATE NATest)
endif()

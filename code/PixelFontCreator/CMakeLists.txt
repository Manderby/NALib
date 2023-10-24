cmake_minimum_required(VERSION 3.13)
project(PixelFontCreator)

set(TARGET_NAME PixelFontCreator)
add_executable(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
  # multi processor compilation
  target_compile_options(${TARGET_NAME} PRIVATE /MP)

  # working directory
  set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $(OutDir))

  set(OUTPUT_DIRECTORY $(OutDir))

else()

  set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$CONFIGURATION/)

endif()



# copying the test images
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/res/miniascii.raw
      \"${OUTPUT_DIRECTORY}miniascii.raw\"
)



# ######### Sources ################

set(sourceFiles
  src/main.c
)

source_group("src" FILES ${sourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${sourceFiles})



# ######### Linking to NALib ################

# Define GUI and OpenGL to be not in use and hide the CMake GUI options.
set(COMPILE_NALIB_GUI 0 INTERNAL FORCE)
set(COMPILE_NALIB_OPENGL 0 INTERNAL FORCE)

set(NALIB_DIR "../NALib")
target_include_directories(${TARGET_NAME} PRIVATE ${NALIB_DIR}/src)
add_subdirectory(${NALIB_DIR} NALib)
target_link_libraries(${TARGET_NAME} PRIVATE NALib)


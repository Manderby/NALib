set(TARGET_NAME Examples)
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
      ${CMAKE_CURRENT_SOURCE_DIR}/res/JSONinput.txt
      \"${OUTPUT_DIRECTORY}JSONinput.txt\"
)



# ######### Sources ################

set(sourceFiles
  src/generalExample.c
  src/jsonExample.c
  src/listExample.c
  src/main.c
  src/stackExample.c
)

source_group("src" FILES ${sourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${sourceFiles})



# ######### Linking to libraries ################

set(NALIB_DIR "../NALib")
target_include_directories(${TARGET_NAME} PRIVATE ${NALIB_DIR}/src)
add_subdirectory(${NALIB_DIR} NALib)
target_link_libraries(${TARGET_NAME} PRIVATE NALib)


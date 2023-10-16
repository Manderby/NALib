set(TARGET_NAME GUIExamples)
add_executable(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(-DNA_COMPILE_GUI=1)
add_definitions(-DNA_COMPILE_OPENGL=1)

if (MSVC)
  add_definitions(-DNA_USE_WINDOWS_COMMON_CONTROLS_6=1)
  
  # multi processor compilation
  target_compile_options(${TARGET_NAME} PRIVATE /MP)
  # unicode. Theoretically, needs _MBCS to be undefined, but cmake does that.
  add_definitions(-D_UNICODE)

  # working directory
  set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  set(OUTPUT_DIRECTORY $(OutDir))

else()

  set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$CONFIGURATION)

endif()



# copying the test images
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/res/icon.png
      \"${OUTPUT_DIRECTORY}/icon.png\"
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/res/man.png
      \"${OUTPUT_DIRECTORY}/man.png\"
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/res/man2.png
      \"${OUTPUT_DIRECTORY}/man2.png\"
)



# ######### Sources ################

set(sourceFiles
  src/ButtonController.c
  src/ExperimentController.c
  src/FontController.c
  src/GUIExamples.h
  src/GUIExamplesApplication.c
  src/main.c
  src/TemperatureController.c
  src/translationIDs.h
)

set(resourceFiles
  res/translationsFinance.txt
  res/translationsGeneral.txt
)

set(macOSResourceFiles
  res/macOS/GUIExamples.entitlements
  res/macOS/Info.plist
  res/macOS/MainMenu.xib
)

source_group("src" FILES ${sourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${sourceFiles})

source_group("res" FILES ${resourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${resourceFiles})

source_group("res/macOS" FILES ${macOSResourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${macOSResourceFiles})



# ######### Linking to libraries ################

set(NALIB_DIR "../NALib")
add_subdirectory(${NALIB_DIR} NALib)
target_link_libraries(${TARGET_NAME} PRIVATE NALib)

find_package(OpenGL REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE ${OPENGL_gl_LIBRARY})

if (MSVC)
  target_link_libraries(${TARGET_NAME} PRIVATE ComCtl32.lib)
else()
  #find_library(CARBON_FRAMEWORK Carbon)
  #if (NOT CARBON_FRAMEWORK)
  #    message(FATAL_ERROR "Carbon not found")
  #endif()
  #target_link_libraries(${TARGET_NAME} PRIVATE ${CARBON_FRAMEWORK})

  #find_library(COCOA_FRAMEWORK Cocoa)
  #if (NOT COCOA_FRAMEWORK)
  #    message(FATAL_ERROR "Cocoa not found")
  #endif()
  #target_link_libraries(${TARGET_NAME} PRIVATE ${COCOA_FRAMEWORK})

  #target_link_libraries(${TARGET_NAME} PRIVATE Carbon/Carbon.h)
  #target_link_libraries(${TARGET_NAME} PRIVATE Cocoa/Cocoa.h)
  #target_link_libraries(${TARGET_NAME} PRIVATE UniformTypeIdentifiers)
  
  FIND_LIBRARY(CARBON_FRAMEWORK Carbon)
  FIND_LIBRARY(COCOA_FRAMEWORK Cocoa)
  FIND_LIBRARY(UTYPE_FRAMEWORK UniformTypeIdentifiers)
  target_link_libraries(${TARGET_NAME} PRIVATE ${CARBON_FRAMEWORK})
  target_link_libraries(${TARGET_NAME} PRIVATE ${COCOA_FRAMEWORK})
  target_link_libraries(${TARGET_NAME} PRIVATE ${UTYPE_FRAMEWORK})
  
endif()

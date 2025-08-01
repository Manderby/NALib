cmake_minimum_required(VERSION 3.13)
project(GUIExamples)

set(TARGET_NAME GUIExamples)

add_executable(${TARGET_NAME})

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
  add_definitions(-DNA_USE_WINDOWS_COMMON_CONTROLS_6=1)
  
  # multi processor compilation
  target_compile_options(${TARGET_NAME} PRIVATE /MP)
  # unicode. Theoretically, needs _MBCS to be undefined, but cmake does that.
  add_definitions(-D_UNICODE)

  # working directory
  set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $(OutDir))

  set(OUTPUT_DIRECTORY $(OutDir))

else()

  set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$CONFIGURATION/)

endif()



# copying the test images
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/res/icon.png
      \"${OUTPUT_DIRECTORY}icon.png\"
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/res/man.png
      \"${OUTPUT_DIRECTORY}man.png\"
  COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/res/man2.png
      \"${OUTPUT_DIRECTORY}man2.png\"
)



# ######### Sources ################

set(controllerSourceFiles
  src/Controller/ApplicationController.c
  src/Controller/ButtonController.c
  src/Controller/CheckBoxController.c
  src/Controller/FontController.c
  src/Controller/ImageSpaceController.c
  src/Controller/LabelController.c
  src/Controller/OpenGLSpaceController.c
  src/Controller/MenuController.c
  src/Controller/MetalSpaceController.c
  src/Controller/RadioController.c
  src/Controller/ScreenController.c
  src/Controller/SelectController.c
  src/Controller/SliderController.c
  src/Controller/SpaceController.c
  src/Controller/TextBoxController.c
  src/Controller/TextFieldController.c
  src/Controller/WindowController.c
)

set(sourceFiles
  src/CommonController.c
  src/CommonController.h
  src/ExampleController.c
  src/GUIExamples.h
  src/GUIExamplesApplication.c
  src/Layout.h
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

source_group("src/Controller" FILES ${controllerSourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${controllerSourceFiles})

source_group("res" FILES ${resourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${resourceFiles})

source_group("res/macOS" FILES ${macOSResourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${macOSResourceFiles})



# ######### Linking to NALib ################

# Define GUI, OpenGL and Metal to be in use and hide the CMake GUI options.
add_definitions(-DNA_COMPILE_GUI=1)
set(COMPILE_NALIB_GUI 1 CACHE INTERNAL "GUI required")

add_definitions(-DNA_COMPILE_OPENGL=1)
set(COMPILE_NALIB_OPENGL 1 CACHE INTERNAL "OpenGL required")

if (MSVC)
else()
	add_definitions(-DNA_COMPILE_METAL=1)
	set(COMPILE_NALIB_METAL 1 CACHE INTERNAL "Metal required")
endif()

# Adding NALib.
set(NALIB_DIR "../NALib")
target_include_directories(${TARGET_NAME} PRIVATE ${NALIB_DIR}/src)
add_subdirectory(${NALIB_DIR} NALib)
target_link_libraries(${TARGET_NAME} PRIVATE NALib)



# ######### Linking to other libraries ################

# These lines are necessary to include OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE ${OPENGL_gl_LIBRARY})

if (MSVC)
  target_link_libraries(${TARGET_NAME} PRIVATE ComCtl32.lib)
else()
	# These lines are necessary to include Metal
	target_link_libraries(${TARGET_NAME} PRIVATE "-framework Metal")
	target_link_libraries(${TARGET_NAME} PRIVATE "-framework QuartzCore")

  FIND_LIBRARY(CARBON_FRAMEWORK Carbon)
  FIND_LIBRARY(COCOA_FRAMEWORK Cocoa)
  FIND_LIBRARY(UTYPE_FRAMEWORK UniformTypeIdentifiers)
  target_link_libraries(${TARGET_NAME} PRIVATE ${CARBON_FRAMEWORK})
  target_link_libraries(${TARGET_NAME} PRIVATE ${COCOA_FRAMEWORK})
  target_link_libraries(${TARGET_NAME} PRIVATE ${UTYPE_FRAMEWORK})
  
endif()

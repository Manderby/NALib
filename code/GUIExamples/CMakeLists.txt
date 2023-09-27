set(TARGET_NAME GUIExamples)
add_executable(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(-DNA_COMPILE_GUI=1)
add_definitions(-DNA_COMPILE_OPENGL=1)
add_definitions(-DNA_USE_WINDOWS_COMMON_CONTROLS_6=0)

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

set(NALIB_DIR "../NALib")
add_subdirectory(${NALIB_DIR} NALib)
target_link_libraries(${TARGET_NAME} NALib)

find_package(OpenGL REQUIRED)
target_link_libraries(${TARGET_NAME} ${OPENGL_gl_LIBRARY})

target_link_libraries(${TARGET_NAME} ComCtl32.lib)

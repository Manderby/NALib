add_executable(GUIExamples)

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(-DNA_COMPILE_GUI=1)
add_definitions(-DNA_COMPILE_OPENGL=1)



set(sourceFiles
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
target_sources(GUIExamples PRIVATE ${sourceFiles})

source_group("res" FILES ${resourceFiles})
target_sources(GUIExamples PRIVATE ${resourceFiles})

source_group("res/macOS" FILES ${macOSResourceFiles})
target_sources(GUIExamples PRIVATE ${macOSResourceFiles})

set(NALIB_DIR "../NALib")
add_subdirectory(${NALIB_DIR} NALib)

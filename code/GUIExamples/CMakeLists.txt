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
  set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")
  
  # copying the test images
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/res/icon.png
        \"$(OutDir)/icon.png\"
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/res/man.png
        \"$(OutDir)/man.png\"
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/res/man2.png
        \"$(OutDir)/man2.png\"
  )
else()
endif()



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
target_link_libraries(${TARGET_NAME} NALib)

find_package(OpenGL REQUIRED)
target_link_libraries(${TARGET_NAME} ${OPENGL_gl_LIBRARY})

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
else()

endif()

target_link_libraries(${TARGET_NAME} ComCtl32.lib)

cmake_minimum_required(VERSION 3.13)
project(Test)

set(TARGET_NAME Test)
add_executable(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
  # multi processor compilation
  target_compile_options(${TARGET_NAME} PRIVATE /MP)
else()
endif()

# ######### Sources ################

set(testNABaseFiles
  src/testNALib/testNABase/testNAChar.c
  src/testNALib/testNABase/testNACompiler.c
  src/testNALib/testNABase/testNAConfiguration.c
  src/testNALib/testNABase/testNADebugging.c
  src/testNALib/testNABase/testNAEncoding.c
  src/testNALib/testNABase/testNAEnvironment.c
  src/testNALib/testNABase/testNAFloatingPoint.c
  src/testNALib/testNABase/testNAInt64.c
  src/testNALib/testNABase/testNAInt128.c
  src/testNALib/testNABase/testNAInt256.c
  src/testNALib/testNABase/testNAInteger.c
  src/testNALib/testNABase/testNALanguage.c
  src/testNALib/testNABase/testNAMacros.c
  src/testNALib/testNABase/testNANumerics.c
  src/testNALib/testNABase/testNAPointerArithmetics.c
)

set(testNACoreFiles
  src/testNALib/testNACore/testNATesting.c
  src/testNALib/testNACore/testNAValueHelper.c
)

set(testNAStructFiles
  src/testNALib/testNAStruct/testNABuffer.c
  src/testNALib/testNAStruct/testNAStack.c
)

set(testNALibFiles
  src/testNALib/testNABase.c
  src/testNALib/testNACore.c
  src/testNALib/testNAStruct.c
)

set(sourceFiles
  src/mainHeapTest.c
  src/mainTreeTest.c
  src/testNALib.c
)

source_group("src/testNALib/testNABase" FILES ${testNABaseFiles})
target_sources(${TARGET_NAME} PRIVATE ${testNABaseFiles})

source_group("src/testNALib/testNACore" FILES ${testNACoreFiles})
target_sources(${TARGET_NAME} PRIVATE ${testNACoreFiles})

source_group("src/testNALib/testNAStruct" FILES ${testNAStructFiles})
target_sources(${TARGET_NAME} PRIVATE ${testNAStructFiles})

source_group("src/testNALib" FILES ${testNALibFiles})
target_sources(${TARGET_NAME} PRIVATE ${testNALibFiles})

source_group("src" FILES ${sourceFiles})
target_sources(${TARGET_NAME} PRIVATE ${sourceFiles})




# ######### Linking to libraries ################

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../NAlib/src)

set(ENABLE_NALIB_TESTING 1)

set(NALIB_DIR "../NALib")
target_include_directories(${TARGET_NAME} PRIVATE ${NALIB_DIR}/src)
add_subdirectory(${NALIB_DIR} NALib)
target_link_libraries(${TARGET_NAME} PRIVATE NALib)

set(NATEST_DIR "../../lib/NATest/code/NATest")
target_include_directories(${TARGET_NAME} PRIVATE ${NATEST_DIR}/src)
#add_subdirectory(${NATEST_DIR} NATest) # will be added by NALib
target_link_libraries(${TARGET_NAME} PRIVATE NATest)

set(TARGET_NAME NALib)
add_library(NALib STATIC)

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE C)
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (MSVC)
  # multi processor compilation
  target_compile_options(${TARGET_NAME} PRIVATE /MP)
else()
endif()

set(primaryHeaderFile
  src/NALib.h
)

source_group("" FILES ${primaryHeaderFile})
target_sources(${TARGET_NAME} PRIVATE ${primaryHeaderFile})

include(src/NAApp/CMakeSrcList.txt)
include(src/NABase/CMakeSrcList.txt)
include(src/NAMath/CMakeSrcList.txt)
include(src/NAStruct/CMakeSrcList.txt)
include(src/NAUtility/CMakeSrcList.txt)
include(src/NAVisual/CMakeSrcList.txt)

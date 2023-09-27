set(TARGET_NAME NALib)
add_library(NALib STATIC)

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE C)
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(primaryHeaderFile
  src/NALib.h
)

source_group("" FILES ${primaryHeaderFile})
target_sources(${TARGET_NAME} PUBLIC ${primaryHeaderFile})

include(src/NAApp/CMakeSrcList.txt)
include(src/NABase/CMakeSrcList.txt)
include(src/NAMath/CMakeSrcList.txt)
include(src/NAStruct/CMakeSrcList.txt)
include(src/NAUtility/CMakeSrcList.txt)
include(src/NAVisual/CMakeSrcList.txt)

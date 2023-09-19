add_executable(NALib)

set_target_properties(NALib PROPERTIES LINKER_LANGUAGE C)
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(primaryHeaderFile
  src/NALib.h
)

source_group("" FILES ${primaryHeaderFile})
target_sources(NALib PRIVATE ${primaryHeaderFile})

include(src/NAApp/CMakeSrcList.txt)
include(src/NABase/CMakeSrcList.txt)

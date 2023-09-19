add_executable(Examples)

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)



set(sourceFiles
  src/generalExample.c
  src/jsonExample.c
  src/listExample.c
  src/main.c
  src/stackExample.c
)

set(resourceFiles
  res/JSONinput.txt
)

source_group("src" FILES ${sourceFiles})
target_sources(Examples PRIVATE ${sourceFiles})

source_group("res" FILES ${resourceFiles})
target_sources(Examples PRIVATE ${resourceFiles})


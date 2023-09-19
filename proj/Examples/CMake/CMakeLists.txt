cmake_minimum_required(VERSION 3.13)
project(Examples)

set(TARGET_SRC_DIR "../../../code/Examples")
add_subdirectory(${TARGET_SRC_DIR} Examples)

set(TARGET_SRC_DIR "../../../code/NALib")
add_subdirectory(${TARGET_SRC_DIR} NALib)

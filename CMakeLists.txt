cmake_minimum_required(VERSION 3.10)

project(Engine)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

add_subdirectory(engine)
add_subdirectory(app)
add_subdirectory(appGLFW)

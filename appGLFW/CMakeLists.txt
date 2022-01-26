SET(sources
mainProg.cpp)

SET(headers)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

#find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

add_executable(APPENGINEGLFW ${sources} ${headers})

target_include_directories(APPENGINEGLFW PUBLIC ENGINE
${OPENGL_INCLUDE_DIR}
${GLFW3_INCLUDE_DIR})

target_link_libraries(APPENGINEGLFW ENGINE
${OPENGL_LIBRARIES}
${GLFW3_LIBRARY})
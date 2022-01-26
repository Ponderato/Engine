SET(sources
mainProgram.cpp)

SET(headers)

#find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(APPENGINE ${sources} ${headers})

target_include_directories(APPENGINE PUBLIC ENGINE
${OPENGL_INCLUDE_DIR}
${GLUT_INCLUDE_DIR})

target_link_libraries(APPENGINE ENGINE
${OPENGL_LIBRARIES}
${GLUT_LIBRARIES})
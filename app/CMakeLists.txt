SET(sources
mainProgram.cpp)

SET(headers)

find_package(GLUT REQUIRED)

add_executable(APPENGINE ${sources} ${headers})

target_include_directories(APPENGINE PUBLIC ENGINE
${GLUT_INCLUDE_DIR})

target_link_libraries(APPENGINE ENGINE
${GLUT_LIBRARIES})
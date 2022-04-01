SET(sources
mainProg.cpp
)

SET(headers
)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(GLFW3 REQUIRED)

add_executable(APPENGINEGLFW ${sources} ${headers})

target_include_directories(APPENGINEGLFW PUBLIC ENGINE
${GLFW3_INCLUDE_DIR})

target_link_libraries(APPENGINEGLFW ENGINE
${GLFW3_LIBRARY})
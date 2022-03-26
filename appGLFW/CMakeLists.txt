SET(sources
mainProg.cpp
)

SET(headers
)
#stb_image.h and .cpp to use the library

find_package(GLFW3 REQUIRED)

add_executable(APPENGINEGLFW ${sources} ${headers})

target_include_directories(APPENGINEGLFW PUBLIC ENGINE
${GLFW3_INCLUDE_DIR})

target_link_libraries(APPENGINEGLFW ENGINE
${GLFW3_LIBRARY})
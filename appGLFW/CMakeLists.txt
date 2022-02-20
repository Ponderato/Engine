SET(sources
mainProg.cpp
stb_image.cpp
Program.cpp
Mesh.cpp)

SET(headers
stb_image.h
Program.h
Mesh.h)
#stb_image.h and .cpp to use the library

find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
find_package(Assimp REQUIRED)

add_executable(APPENGINEGLFW ${sources} ${headers})

target_include_directories(APPENGINEGLFW PUBLIC ENGINE
${GLFW3_INCLUDE_DIR}
${GLM_INCLUDE_DIRS}
${ASSIMP_INCLUDE_DIRS})

target_link_libraries(APPENGINEGLFW ENGINE
${GLFW3_LIBRARY}
${ASSIMP_LIBRARIES})
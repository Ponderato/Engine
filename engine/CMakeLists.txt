SET(sources
Context.cpp
stb_image.cpp
Program.cpp
Mesh.cpp
Cube.cpp
Camera.cpp)

SET(headers
Context.h
stb_image.h
Program.h
Mesh.h
Cube.h
Camera.h)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(Assimp REQUIRED)

add_library(ENGINE STATIC ${sources} ${headers})

target_include_directories(ENGINE PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}
${OPENGL_INCLUDE_DIR}
${GLEW_INCLUDE_DIRS}
${GLM_INCLUDE_DIRS}
${ASSIMP_INCLUDE_DIRS})

target_link_libraries(ENGINE 
${OPENGL_LIBRARIES}
${GLEW_LIBRARIES}
${ASSIMP_LIBRARIES})
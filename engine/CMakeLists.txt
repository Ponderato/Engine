SET(sources
oglContext.cpp
stb_image.cpp
Program.cpp
Mesh.cpp
Cube.cpp
Camera.cpp)

SET(headers
oglContext.h
stb_image.h
Program.h
Mesh.h
Cube.h
Camera.h)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(Assimp REQUIRED)

add_library(ENGINE ${sources} ${headers})

target_include_directories(ENGINE PUBLIC 
${OPENGL_INCLUDE_DIR}
${GLEW_INCLUDE_DIRS}
${GLM_INCLUDE_DIRS}
${PROJECT_SOURCE_DIR}
${ASSIMP_INCLUDE_DIRS})

target_link_libraries(ENGINE 
${OPENGL_LIBRARIES}
${GLEW_LIBRARIES}
${ASSIMP_LIBRARIES})
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
Camera.h
)

#Shaders filter on solution explorer
SET(Shaders_SRC
	${CMAKE_SOURCE_DIR}/engine/Shaders/)

FILE(GLOB_RECURSE Shaders
	${Shaders_SRC}/*.glsl*
)
#---------------------------------------------

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(Assimp REQUIRED)

#To make the Shaders filter appear in the solution explorer, it has
#to be added here
add_library(ENGINE STATIC ${sources} ${headers} ${Shaders})

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

#Shaders filter on solution explorer
source_group("Shaders" FILES  ${Shaders})

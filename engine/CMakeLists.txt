SET(sources
oglContext.cpp
)

SET(headers
oglContext.h
)


find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_library(ENGINE ${sources} ${headers})

target_include_directories(ENGINE PUBLIC 
${OPENGL_INCLUDE_DIR}
${GLEW_INCLUDE_DIRS}
${PROJECT_SOURCE_DIR})

target_link_libraries(ENGINE 
${OPENGL_LIBRARIES}
${GLEW_LIBRARIES}
)
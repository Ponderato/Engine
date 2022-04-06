SET(sources
mainProg.cpp
)

SET(headers
)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

file(COPY ${CMAKE_SOURCE_DIR}/engine/Shaders/
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(GLFW3 REQUIRED)

add_executable(APPENGINEGLFW ${sources} ${headers})

target_include_directories(APPENGINEGLFW PUBLIC ENGINE
${GLFW3_INCLUDE_DIR})

target_link_libraries(APPENGINEGLFW ENGINE
${GLFW3_LIBRARY})
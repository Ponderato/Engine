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

#Add the shaders to binary each time the app is built. That way 
#we can make changes in the sahders and there is no need to recomile them
add_custom_target(copy_shaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/engine/Shaders/
	${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Copying shaders into binary directory")

add_dependencies(APPENGINEGLFW copy_shaders)

#Same with the dlls
add_custom_target(copy_dlls ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/dlls/
	${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Copying dlls into binary directory")

add_dependencies(APPENGINEGLFW copy_dlls)

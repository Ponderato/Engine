SET(sources
mainProg.cpp
HierarchyPanel.cpp
RenderPanel.cpp
InspectorPanel.cpp
Panel.cpp
${CMAKE_SOURCE_DIR}/imgui/imgui_impl_glfw.cpp
${CMAKE_SOURCE_DIR}/imgui/imgui_impl_opengl3.cpp
)

SET(headers
HierarchyPanel.h
RenderPanel.h
InspectorPanel.h
Panel.h
${CMAKE_SOURCE_DIR}/imgui/imgui_impl_glfw.h
${CMAKE_SOURCE_DIR}/imgui/imgui_impl_opengl3.h
${CMAKE_SOURCE_DIR}/imgui/imgui_impl_opengl3_loader.h
)

FILE(GLOB_RECURSE imgui
	${CMAKE_SOURCE_DIR}/imgui/imconfig.h
	${CMAKE_SOURCE_DIR}/imgui/imgui_internal.h
	${CMAKE_SOURCE_DIR}/imgui/imgui.h
	${CMAKE_SOURCE_DIR}/imgui/imgui.cpp
	${CMAKE_SOURCE_DIR}/imgui/imgui_demo.cpp
	${CMAKE_SOURCE_DIR}/imgui/imgui_draw.cpp
	${CMAKE_SOURCE_DIR}/imgui/imgui_tables.cpp
	${CMAKE_SOURCE_DIR}/imgui/imgui_widgets.cpp
	)
	
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(GLFW3 REQUIRED)

add_executable(APPENGINEGLFW ${sources} ${headers} ${imgui})

target_include_directories(APPENGINEGLFW PUBLIC ENGINE
${GLFW3_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/imgui
)

target_link_libraries(APPENGINEGLFW ENGINE
${GLFW3_LIBRARY}
)

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

source_group("Imgui" FILES  ${imgui})

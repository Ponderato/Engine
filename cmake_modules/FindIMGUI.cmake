if (TARGET imgui)
    return()
endif()


set(IMGUI_ROOT_DIR ${CMAKE_SOURCE_DIR}/imgui)

set(_imgui_SourceDir ${IMGUI_ROOT_DIR})

project(IMGUI VERSION 1.76)

set(_imgui_Sources
    ${IMGUI_ROOT_DIR}/imconfig.h
    ${IMGUI_ROOT_DIR}/imgui.cpp
    ${IMGUI_ROOT_DIR}/imgui.h
    ${IMGUI_ROOT_DIR}/imgui_draw.cpp
    ${IMGUI_ROOT_DIR}/imgui_internal.h
    ${IMGUI_ROOT_DIR}/imgui_widgets.cpp
    ${IMGUI_ROOT_DIR}/imgui_tables.cpp
    ${IMGUI_ROOT_DIR}/imstb_rectpack.h
    ${IMGUI_ROOT_DIR}/imstb_textedit.h
    ${IMGUI_ROOT_DIR}/imstb_truetype.h
	${IMGUI_ROOT_DIR}/imgui_demo.cpp
	${IMGUI_ROOT_DIR}/imgui_impl_glfw.cpp
	${IMGUI_ROOT_DIR}/imgui_impl_opengl3.cpp

)

add_library(${PROJECT_NAME} STATIC ${_imgui_Sources})

target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_ROOT_DIR})
#target_compile_definitions(${PROJECT_NAME} PUBLIC "ImDrawIdx=unsigned int")

source_group(TREE ${IMGUI_ROOT_DIR} FILES ${_imgui_Sources})

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "external")



include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)

find_package_handle_standard_args(
    imgui
    REQUIRED_VARS
        _imgui_SourceDir
)


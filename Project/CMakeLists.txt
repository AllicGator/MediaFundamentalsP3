cmake_minimum_required(VERSION 3.13)	
project(KirkbyMediaFundamentalsP1)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Available build-types: Debug, Release" FORCE)

file(GLOB media_fundamentals_SRC/KirkbyMediaFundamentalsP1  
    "*.c*"
)

file(
	GLOB_RECURSE _source_list
	LIST_DIRECTORIES false
	"${PROJECT_SOURCE_DIR}/common/src/*.c*"
	"${PROJECT_SOURCE_DIR}/common/src/*.h*"
	"${PROJECT_SOURCE_DIR}/common/src/*.glsl"
	"${PROJECT_SOURCE_DIR}/common/src/*.frag"
	"${PROJECT_SOURCE_DIR}/common/src/*.vert"
	"${PROJECT_SOURCE_DIR}/KirkbyMediaFundamentalsP1/*.h"
	"${PROJECT_SOURCE_DIR}/KirkbyMediaFundamentalsP1/*.cpp"
	"${PROJECT_SOURCE_DIR}/KirkbyMediaFundamentalsP1/config_files/*.xml"
)

include_directories(${PROJECT_SOURCE_DIR}/common/includes/)


foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${PROJECT_SOURCE_DIR}/common/src" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()


add_executable(KirkbyMediaFundamentalsP1 ${media_fundamentals_SRC} ${_source_list})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT KirkbyMediaFundamentalsP1)

target_link_libraries(KirkbyMediaFundamentalsP1 ${PROJECT_SOURCE_DIR}/common/lib/x64/Debug/fmod_vc.lib)
file( COPY common/lib/x64/Debug/fmod.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})	

target_link_libraries(KirkbyMediaFundamentalsP1 ${PROJECT_SOURCE_DIR}/common/lib//x64/Debug/glfw3.lib)
file( COPY common/lib/x64/Debug/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})	

target_link_libraries(KirkbyMediaFundamentalsP1 ${PROJECT_SOURCE_DIR}/common/lib//x64/Debug/freetype.lib)
file( COPY common/lib/x64/Debug/freetype.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})	

target_link_libraries(KirkbyMediaFundamentalsP1 ${PROJECT_SOURCE_DIR}/common/lib/x64/Release/fmodL_vc.lib)
file( COPY common/lib/x64/Release/fmodL.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})	
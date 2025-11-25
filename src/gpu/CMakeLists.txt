cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

include(HelloGpuCommon)

find_package(SDL3 CONFIG REQUIRED)

add_library(Renderer STATIC)

hellogpu_init_target(Renderer)

target_sources(Renderer
	PRIVATE
		$<$<STREQUAL:${INFINITELOOP_GRAPHICS_API},d3d12>:
			${CMAKE_CURRENT_SOURCE_DIR}/d3d12/d3d12_device.cpp
		>
		$<$<STREQUAL:${INFINITELOOP_GRAPHICS_API},vulkan>:
			${CMAKE_CURRENT_SOURCE_DIR}/vulkan/vulkan_device.cpp
		>
		${CMAKE_CURRENT_SOURCE_DIR}/gpu.cpp

	PUBLIC
		FILE_SET CXX_MODULES
		FILES
			${CMAKE_CURRENT_SOURCE_DIR}/device.ixx
			${CMAKE_CURRENT_SOURCE_DIR}/gpu.ixx
)

target_link_libraries(Renderer PUBLIC HelloGpuEastl)
target_link_libraries(Renderer PRIVATE SDL3::SDL3)
cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

include(HelloGpuCommon)

find_package(SDL3 CONFIG REQUIRED)

add_library(gpu STATIC)

hellogpu_init_target(gpu)

target_sources(gpu
	PRIVATE
		$<$<STREQUAL:${HELLOGPU_GRAPHICS_API},d3d12>:
			${CMAKE_CURRENT_SOURCE_DIR}/d3d12/d3d12_device.cpp
		>
		$<$<STREQUAL:${HELLOGPU_GRAPHICS_API},vulkan>:
			${CMAKE_CURRENT_SOURCE_DIR}/vulkan/vulkan_device.cpp
		>
		${CMAKE_CURRENT_SOURCE_DIR}/gpu.cpp

	PUBLIC
		FILE_SET CXX_MODULES
		FILES
			${CMAKE_CURRENT_SOURCE_DIR}/device.ixx
			${CMAKE_CURRENT_SOURCE_DIR}/gpu.ixx
)

target_link_libraries(gpu PUBLIC HelloGpuEastl)
target_link_libraries(gpu PRIVATE SDL3::SDL3)
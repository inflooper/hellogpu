cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

include(HelloGpuCommon)

find_package(EASTL CONFIG REQUIRED)

add_library(HelloGpuEastl STATIC)

hellogpu_init_target(HelloGpuEastl)

set(srcDir "${PROJECT_SOURCE_DIR}/src/eastl")

target_sources(HelloGpuEastl
	PRIVATE
		eastl.cpp

	PUBLIC
		FILE_SET CXX_MODULES
		FILES
			eastl.ixx
)

target_link_libraries(HelloGpuEastl PRIVATE EASTL)
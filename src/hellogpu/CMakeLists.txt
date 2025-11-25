cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

include(HelloGpuCommon)

find_package(SDL3 CONFIG REQUIRED)

add_executable(HelloGpu WIN32)

hellogpu_init_target(HelloGpu)

target_compile_options(HelloGpu PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

set(srcDir "${PROJECT_SOURCE_DIR}/src/hellogpu")

target_sources(HelloGpu
	PRIVATE
		main.cpp
)

target_link_libraries(HelloGpu PRIVATE HelloGpuEastl)
target_link_libraries(HelloGpu PRIVATE SDL3::SDL3)

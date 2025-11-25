cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake")

include(FetchContent)

FetchContent_Declare(
  WindowsToolchain
  GIT_REPOSITORY https://github.com/MarkSchofield/WindowsToolchain.git
  GIT_TAG        133fa4f3e36f89c44053f42c30a9e07dee8beda9
)
FetchContent_MakeAvailable(WindowsToolchain)

set(HELLOGPU_GRAPHICS_API "d3d12" CACHE STRING "Graphics api selected at cmake configure time")

set(HELLOGPU_GRAPHICS_API_VALUES "d3d12;vulkan" CACHE STRING "Available graphics apis")

set_property(CACHE HELLOGPU_GRAPHICS_API PROPERTY STRINGS ${HELLOGPU_GRAPHICS_API_VALUES})

if(NOT HELLOGPU_GRAPHICS_API IN_LIST HELLOGPU_GRAPHICS_API_VALUES)
	message(FATAL_ERROR "Invalid GraphicsApi '${HELLOGPU_GRAPHICS_API}'. Must be one of the following values: ${HELLOGPU_GRAPHICS_API_VALUES}")
else()
	message(STATUS "GraphicsApi=${HELLOGPU_GRAPHICS_API}")
endif()

project(HelloGpu LANGUAGES C CXX)

add_subdirectory("src/eastl")
add_subdirectory("src/gpu")
add_subdirectory("src/hellogpu")
cmake_minimum_required(VERSION 2.6)

# project configuration
project(sudokuSolver)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

# add includes
SET(INCLUDE_DIR include)
include_directories(${PROJECT_SOURCE_DIR}/${INCLUDE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/${INCLUDE_DIR})

# get sources files
file(
	GLOB_RECURSE
	sources_files
	src/*
)

add_executable(
	sudokuSolver
	${sources_files}
)
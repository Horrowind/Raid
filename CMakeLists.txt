cmake_minimum_required (VERSION 2.8.3)
project(raid)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=gnu99 -D_FILE_OFFSET_BITS=64")
set(SOURCE_DIR src)

find_package(FUSE 2.6 REQUIRED)

if(FUSE_FOUND)
	add_definitions(${FUSE_DEFINITIONS})
	include_directories(${FUSE_INCLUDE_DIRS})
	
	if (CMAKE_BUILD_TYPE STREQUAL Debug)
		set(BUILD_POSTFIX "_d")
	else(CMAKE_BUILD_TYPE STREQUAL Debug)
		set(BUILD_POSTFIX "")
	endif(CMAKE_BUILD_TYPE STREQUAL Debug)

	set_source_files_properties(galois.c PROPERTIES COMPILE_FLAGS "-Wno-unused-variable -Wno-unused-but-set-variable")

	add_library(galois galois.c)
	add_library(raidconf raidconf.c)

	add_executable(raidcreate raidcreate.c)
	add_executable(raidassemble raidassemble.c)

	target_link_libraries(raidcreate galois raidconf)
	target_link_libraries(raidassemble ${FUSE_LIBRARIES} raidconf)

endif(FUSE_FOUND)

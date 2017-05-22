#
# Copyright 2016 Jerry Jacobs. All rights reserved.
# Use of this source code is governed by the MIT
# license that can be found in the LICENSE file.
#
cmake_minimum_required(VERSION 2.8)
project(stlink2 C)

include(ExternalProject)
include(CheckFunctionExists)

if (POLICY CMP0042)
	# Newer cmake on MacOS should use @rpath
	cmake_policy (SET CMP0042 NEW)
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS On)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(cmake/Version.txt)
include(cmake/Doxygen.txt)
include(cmake/CheckCCompilerFlag.txt)

set (LUA_LIBRARY lua)

ExternalProject_Add(
	${LUA_LIBRARY}
	SOURCE_DIR "${PROJECT_SOURCE_DIR}/3thparty/lua.org/lua-5.3.3"
	INSTALL_COMMAND ""
)

externalproject_get_property(${LUA_LIBRARY} install_dir)
set(LUA_INCLUDE "${PROJECT_SOURCE_DIR}/3thparty/lua.org/lua-5.3.3/src")
if (NOT WIN32)
	set(LUA_LIBRARY_STATIC ${install_dir}/src/lua-build/liblua.a)
else()
	set(LUA_LIBRARY_STATIC ${install_dir}/src/lua-build/lua.lib)
endif()
include_directories(${LUA_INCLUDE})

# fixup __FILE__ absolute paths in logging module
# see: https://cmake.org/pipermail/cmake-developers/2015-January/024202.html
string(LENGTH "${CMAKE_SOURCE_DIR}/" CMAKE_SOURCE_DIR_LENGTH)
add_definitions(-DCMAKE_SOURCE_DIR_LENGTH=${CMAKE_SOURCE_DIR_LENGTH})

if (WIN32)
	add_definitions("-DSTLINK2_HAVE_WINDOWS")
	add_definitions("-Dsnprintf=_snprintf")
	add_definitions("-D__func__=__FUNCTION__") # MSVC doesn't support C99
	add_definitions("-D_WINSOCKAPI_")
	add_cflag_if_supported("/W4")
else ()
	check_function_exists(nanosleep STLINK2_HAVE_NANOSLEEP)
	add_definitions (-D_GNU_SOURCE)
	add_cflag_if_supported("-Wall")
	add_cflag_if_supported("-fPIC")

	if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
		add_cflag_if_supported("-O0")
		add_cflag_if_supported("-ggdb")
		add_cflag_if_supported("-fprofile-arcs")
		add_cflag_if_supported("-ftest-coverage")
		add_cflag_if_supported("-fno-omit-frame-pointer")

		set(STLINK2_VERSION "${STLINK2_VERSION}-debug")
		add_definitions("-DSTLINK2_DEBUG")
		include(CTest)
	else()
		add_cflag_if_supported("-O2")
		add_cflag_if_supported("-Werror")
		add_cflag_if_supported("-fomit-frame-pointer")
		add_cflag_if_supported("-fvisibility=hidden")
	endif()
endif()

if(MSVC)
	add_cflag_if_supported("/MD")
endif()

add_subdirectory(include)
include_directories(include ${CMAKE_BINARY_DIR}/include)
add_subdirectory(src)

###
# Lint
###
if (BUILD_TESTING)
	include(cmake/Lint.txt)
	add_lint_target("${STLINK2_HEADERS};${STLINK2_HEADERS_INTERNAL}")
endif()

add_subdirectory("tests")

include(cmake/CPackConfig.txt)
include(CPack)

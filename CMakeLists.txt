cmake_minimum_required (VERSION 2.6)
project (autotester)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)

# Set autotester lib os version
if (WIN32)
    SET(CMAKE_FIND_LIBRARY_PREFIXES "")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
    set(AUTOTESTER_ROOT "${CMAKE_CURRENT_LIST_DIR}/lib/autotester/win")
elseif (APPLE)
    set(AUTOTESTER_ROOT "${CMAKE_CURRENT_LIST_DIR}/lib/autotester/apple")
elseif (UNIX)
    set(AUTOTESTER_ROOT "${CMAKE_CURRENT_LIST_DIR}/lib/autotester/unix")
endif()

# Set warnings
SET(UNIX_WARNING_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")
SET(MSVC_WARNING_FLAGS "/permissive- /W4 /w14640")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  ADD_DEFINITIONS(${UNIX_WARNING_FLAGS})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  ADD_DEFINITIONS(${MSVC_WARNING_FLAGS})
endif()

# Get project files
file(GLOB_RECURSE
     ALL_CXX_SOURCE_FILES
	 ${CMAKE_SOURCE_DIR}/src/spa/src/*.h
	 ${CMAKE_SOURCE_DIR}/src/spa/src/*.cpp
	 ${CMAKE_SOURCE_DIR}/src/unit_testing/src/*.h
	 ${CMAKE_SOURCE_DIR}/src/unit_testing/src/*.cpp
	 ${CMAKE_SOURCE_DIR}/src/integration_testing/src/*.h
	 ${CMAKE_SOURCE_DIR}/src/integration_testing/src/*.cpp
     )

# Clang-tidy
if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_BIN clang-tidy-7)
    if(CLANG_TIDY_BIN STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate clang-tidy-7")
    endif()
    list(APPEND CLANG_TIDY_BIN_ARGS
		-config=''
        -header-filter=.*
		-p ${CMAKE_BINARY_DIR}
		${ALL_CXX_SOURCE_FILES}
    )
    add_custom_target(
        tidy
        COMMAND ${CLANG_TIDY_BIN} ${CLANG_TIDY_BIN_ARGS}
        COMMENT "running clang-tidy"
    )
	add_custom_target(
        tidy-fix
        COMMAND ${CLANG_TIDY_BIN} -fix ${CLANG_TIDY_BIN_ARGS}
        COMMENT "running clang-tidy fix"
    )
endif()

# Clang-format
if(ENABLE_CLANG_FORMAT)
	find_program(CLANG_FORMAT_BIN clang-format-7)
	if(CLANG_FORMAT_BIN STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate clang-format-7")
    endif()
    list(APPEND CLANG_FORMAT_ARGS
		-i
		--verbose
        --style=file
        ${ALL_CXX_SOURCE_FILES}
    )
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT_BIN} ${CLANG_FORMAT_ARGS}
        COMMENT "running clang-format"
    )

endif()


list(APPEND CMAKE_PREFIX_PATH "${AUTOTESTER_ROOT}")
find_package(Autotester REQUIRED)
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib")#include catch.hpp

add_subdirectory(src/spa)
add_subdirectory(src/autotester)
#add_subdirectory(src/autotester_gui)
add_subdirectory(src/unit_testing)
add_subdirectory(src/integration_testing)


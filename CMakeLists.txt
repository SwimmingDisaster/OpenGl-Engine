cmake_minimum_required(VERSION 3.20)

# set the project name
project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True) 
set(CMAKE_CXX_COMPILER_LAUNCHER "ccache")

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

set(COMPILE_WARNINGS "-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -fno-omit-frame-pointer")

SET(MY_COMPILE_FLAGS "-fsanitize=address -g ${COMPILE_WARNINGS}")
SET(MY_LINK_FLAGS    "-g -fsanitize=address")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MY_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${MY_LINK_FLAGS}")

add_executable(main ${SOURCE_FILES})

target_include_directories(main PUBLIC
                          "${PROJECT_SOURCE_DIR}/include"
                          "${PROJECT_SOURCE_DIR}/include/code"
                          "${PROJECT_SOURCE_DIR}/include/vendor"
						  "${GTK_INCLUDE_DIRS}")

target_link_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/lib/linux ${PROJECT_SOURCE_DIR}/lib/linux/physx)

target_link_libraries(main pthread assimp glfw3 nfd dl X11)
target_link_libraries(main PhysXExtensions_static_64 PhysX_static_64 PhysXPvdSDK_static_64 PhysXVehicle_static_64 PhysXCharacterKinematic_static_64 PhysXExtensions_static_64 PhysXCooking_static_64 PhysXCommon_static_64 PhysXFoundation_static_64 SnippetUtils_static_64 SnippetRender_static_64)
target_link_libraries(main ${GTK_LIBRARIES})

target_precompile_headers(main PUBLIC
	"$<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/include/mypch.h>")

# add the executable

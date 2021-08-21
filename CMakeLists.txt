cmake_minimum_required(VERSION 3.20)

# set the project name
project(main)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True) 

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

SET(MY_COMPILE_FLAGS "-fsanitize=address -Wall -Wextra -Wno-unused-parameter -std=c++20 -g -Wno-unused-variable -Wno-unused-function -fno-omit-frame-pointer")
SET(MY_LINK_FLAGS    "-g -Llibg -fsanitize=address")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MY_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${MY_LINK_FLAGS}")

add_executable(main ${SOURCE_FILES})

target_include_directories(main PUBLIC
                          "${PROJECT_SOURCE_DIR}/include"
                          "${PROJECT_SOURCE_DIR}/include/code"
                          "${PROJECT_SOURCE_DIR}/include/vendor"
						  "${GTK_INCLUDE_DIRS}")

target_link_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/lib/linux ${PROJECT_SOURCE_DIR}/lib/linux/physx)
target_link_libraries(main assimp)
target_link_libraries(main glfw3)
target_link_libraries(main nfd)
target_link_libraries(main dl)
target_link_libraries(main X11)
target_link_libraries(main pthread)
target_link_libraries(main libPhysXExtensions_static_64.a)
target_link_libraries(main libPhysX_static_64.a)
target_link_libraries(main libPhysXPvdSDK_static_64.a)
target_link_libraries(main libPhysXVehicle_static_64.a)
target_link_libraries(main libPhysXCharacterKinematic_static_64.a)
target_link_libraries(main libPhysXExtensions_static_64.a)
target_link_libraries(main libPhysXCooking_static_64.a)
target_link_libraries(main libPhysXCommon_static_64.a)
target_link_libraries(main libPhysXFoundation_static_64.a)
target_link_libraries(main libSnippetUtils_static_64.a)
target_link_libraries(main libSnippetRender_static_64.a)
target_link_libraries(main ${GTK_LIBRARIES})

target_precompile_headers(main PUBLIC
	"$<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/include/mypch.h>")

# add the executable

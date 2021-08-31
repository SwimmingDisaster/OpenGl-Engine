cmake_minimum_required(VERSION 3.20)

if(APPLE)
	message(FATAL_ERROR "Apple is not supported right now.")
endif()

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

project(main)

if(LINUX)
	FIND_PACKAGE(PkgConfig REQUIRED)
	pkg_check_modules(GTK REQUIRED gtk+-3.0)
endif()

# option to use warnings set to true
# set debug and release builds

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True) 
#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(IWYU include-what-you-use
	#-Xiwyu --pch_in_code
	-Xiwyu --prefix_header_includes=remove
	)
	#	-include=/home/ognjen/OpenGl-Engine/build/CMakeFiles/main.dir/cmake_pch.hxx)
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU})


file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)


if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	#string(REPLACE "/Od" "/O2" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
	#string(REPLACE "/Od" "/O2" CMAKE_C_FLAGS "${CMAKE_C_FLAGS_RELEASE}")
	#string(REPLACE "/Od" "/O2" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE}")
	#string(REPLACE "/Od" "/O2" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_RELEASE}")
	#string(REPLACE "/Od" "/O2" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	#string(REPLACE "/Od" "/O2" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")

	set(COMPILE_WARNINGS "/DEBUG:FULL /Zi")
	#set(COMPILE_WARNINGS "/W4 /Zi")
	set(SHARED_FLAGS "/DEBUG:FULL /O2")
	set(DEBUG_FLAGS "")
	set(RELEASE_FLAGS "")
else()
	#string(REPLACE "-O0" "-O3" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
	#string(REPLACE "-O0" "-O3" CMAKE_C_FLAGS "${CMAKE_C_FLAGS_RELEASE}")
	#string(REPLACE "-O0" "-O3" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE}")
	#string(REPLACE "-O0" "-O3" CMAKE_C_FLAGS_DEBUG "{CMAKE_C_FLAGS_RELEASE}")
	#string(REPLACE "-O0" "-O3" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE}")
	#string(REPLACE "-O0" "-O3" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_RELEASE}")

	set(COMPILE_WARNINGS "-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -fno-omit-frame-pointer")
	set(DEBUG_FLAGS "")
	set(RELEASE_FLAGS "")
	#set(SHARED_FLAGS "-g -fsanitize=address");
	set(SHARED_FLAGS "-g")
endif()

SET(MY_COMPILE_FLAGS "${SHARED_FLAGS} ${COMPILE_WARNINGS}")
SET(MY_LINK_FLAGS    "${SHARED_FLAGS}")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MY_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${MY_LINK_FLAGS}")

#SET(CMAKE_CXX_FLAGS  "${MY_COMPILE_FLAGS}")
#SET(CMAKE_EXE_LINKER_FLAGS "${MY_LINK_FLAGS}")

add_executable(main ${SOURCE_FILES})

target_include_directories(main PUBLIC
                          "${PROJECT_SOURCE_DIR}/include"
                          "${PROJECT_SOURCE_DIR}/include/code"
                          "${PROJECT_SOURCE_DIR}/include/vendor"
						  "${GTK_INCLUDE_DIRS}")

target_link_directories(main PRIVATE
	${PROJECT_SOURCE_DIR}/lib/linux 
	${PROJECT_SOURCE_DIR}/lib/linux/physx
	${PROJECT_SOURCE_DIR}/lib/windows/assimp
	${PROJECT_SOURCE_DIR}/lib/windows/dll
	${PROJECT_SOURCE_DIR}/lib/windows/glfw
	${PROJECT_SOURCE_DIR}/lib/windows/nfd
	${PROJECT_SOURCE_DIR}/lib/windows/physx_debug)

target_link_libraries(main assimp glfw3 nfd)
if(WIN32)
target_link_libraries(main 
	PhysXExtensions_static_64 
	PhysX_64 
	PhysXPvdSDK_static_64 
	PhysXVehicle_static_64 
	PhysXCharacterKinematic_static_64 
	PhysXExtensions_static_64 
	PhysXCooking_64 
	PhysXCommon_64 
	PhysXFoundation_64 
	SnippetUtils_static_64 
	SnippetRender_static_64)
else()
target_link_libraries(main 
	PhysXExtensions_static_64 
	PhysX_static_64 
	PhysXPvdSDK_static_64 
	PhysXVehicle_static_64 
	PhysXCharacterKinematic_static_64 
	PhysXExtensions_static_64 
	PhysXCooking_static_64 
	PhysXCommon_static_64 
	PhysXFoundation_static_64 
	SnippetUtils_static_64 
	SnippetRender_static_64)
endif()
if(LINUX)
	target_link_libraries(main ${GTK_LIBRARIES} X11 dl pthread)
endif()

target_precompile_headers(main PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/include/mypch.h>")

cmake_minimum_required(VERSION 3.12)

project(percipio_cam)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 14)

set(CAMPORT_DIR "D:\\camport3_master\\" CACHE  PATH "Percipio SDK Path")
#set(CAMPORT_DIR "D:\\camport3\\" CACHE  PATH "Percipio SDK Path")
#set(CAMPORT_DIR "/home/user/camport3" CACHE  PATH "Percipio SDK Path")
set(CAMPORT_ARCH  CACHE STRING "Percipio SDK Arch")

option(CSHARP_CAM_DEMO "Enable csharp demo." OFF)

if ( (NOT EXISTS "${CAMPORT_DIR}") OR (NOT EXISTS "${CAMPORT_DIR}/include"))
	message(FATAL_ERROR "need set CAMPORT_DIR to sdk directory")
endif()

add_library(tycam SHARED IMPORTED GLOBAL)
if (MSVC)#for windows
    set(CAMPORT_LIB_ROOT "${CAMPORT_DIR}/lib/win/hostapp")
	message("${CAMPORT_LIB_ROOT}")
    if(CMAKE_CL_64) #x64
        set_property(TARGET tycam PROPERTY IMPORTED_LOCATION ${CAMPORT_LIB_ROOT}/x64/tycam.dll)
        set_property(TARGET tycam PROPERTY IMPORTED_IMPLIB  ${CAMPORT_LIB_ROOT}/x64/tycam.lib)
    else()
        set_property(TARGET tycam PROPERTY IMPORTED_LOCATION ${CAMPORT_LIB_ROOT}/x86/tycam.dll)
        set_property(TARGET tycam PROPERTY IMPORTED_IMPLIB ${CAMPORT_LIB_ROOT}/x86/tycam.lib)
    endif()
else(MSVC)
    if(CAMPORT_ARCH)
		if (NOT EXISTS "${CAMPORT_DIR}/lib/linux/lib_${CAMPORT_ARCH}/libtycam.so")
			message(FATAL_ERROR "not supported arch :${CAMPORT_ARCH} ")
		endif()
        set_property(TARGET tycam PROPERTY IMPORTED_LOCATION "${CAMPORT_DIR}/lib/linux/lib_${CAMPORT_ARCH}/libtycam.so")
    else()
		find_library(CAM_LIB_FILEPATH NAMES tycam) 
		if (NOT EXISTS ${CAM_LIB_FILEPATH})
			message(FATAL_ERROR "not found tycam library in system path")
		endif()
		set_target_properties( tycam PROPERTIES IMPORTED_LOCATION ${CAM_LIB_FILEPATH})
    endif()
endif(MSVC)

include_directories("${CAMPORT_DIR}/include")
include_directories("${CAMPORT_DIR}/sample")

find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV library not found")
else()
    include_directories(${OpenCV_INCLUDE_DIRS})
    include_directories(${OpenCV2_INCLUDE_DIRS})
    # if(CMAKE_BUILD_TYPE STREQUAL Debug)
    # link_directories(${OpenCV_LIB_DIR_DBG})
    # else()
    # link_directories(${OpenCV_LIB_DIR_OPT})
    # endif()
    link_directories(${OpenCV_LIB_DIR})
endif()

#SET(TYCAM_LIB ${TARGET})
    
#message("TYCAM:${TYCAM_LIB}")

add_definitions(-DTUYANG_EXPORTS)

add_library(percipio_cam SHARED tuyang.cpp)

target_link_libraries(percipio_cam tycam ${OpenCV_LIBS})

if (CSHARP_CAM_DEMO)
    ExternalProject_Add(csharp_demo
	    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/csharp"
		BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/csharp_build"
		DEPENDS 
		    percipio_cam
		CMAKE_ARGS 
        -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install_cache
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_STATIC_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}
        -DCMAKE_STATIC_LINKER_FLAGS_DEBUG=${CMAKE_EXE_LINKER_FLAGS_DEBUG}
        -DCMAKE_STATIC_LINKER_FLAGS_RELEASE=${CMAKE_EXE_LINKER_FLAGS_RELEASE}
    	-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DCMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}
        -DCMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}
        -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
        -DCMAKE_C_FLAGS_DEBUG=${CMAKE_C_FLAGS_DEBUG}
        -DCMAKE_C_FLAGS_RELEASE=${CMAKE_C_FLAGS_RELEASE} 
    	-DCMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE=${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}
		-DPCAM_LIB_SUPPORT_FILE_DIR=${PCAM_LIB_SUPPORT_FILE_DIR}
	    )
endif()

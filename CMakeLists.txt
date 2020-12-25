cmake_minimum_required(VERSION 3.12)

project(percipio_cam)

set(CMAKE_CXX_STANDARD 14)

set(CAMPORT_DIR "" CACHE  PATH "Percipio SDK Path")
#set(CAMPORT_DIR "D:\\camport3\\" CACHE  PATH "Percipio SDK Path")
#set(CAMPORT_DIR "/home/user/camport3" CACHE  PATH "Percipio SDK Path")
set(CAMPORT_ARCH  CACHE STRING "Percipio SDK Arch")

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

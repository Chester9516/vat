cmake_minimum_required(VERSION 3.16)

project(tsduck-vatek)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(TSDUCK_VATEK_PLUGIN	tsplugin_vatek)
set(TSDUCK_VATEK_TOOL tsvatek)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDK2_DIR "../../" CACHE STRING "default vatek_sdk_2 folder")
set(TSDUCK_BUILDCONFIG "Debug" CACHE STRING "build configure type")
set(TSDUCK_DIR "TSDuck" CACHE STRING "default tsduck source folder")
set(SDK2_EN_STATIC_ONLY ON)
set(SDK2_EN_QT OFF)
set(SDK2_EN_APP OFF)
set(SDK2_EN_SAMPLE OFF)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SDK2_DIR}/)
    message(FATAL_ERROR "vatek_sdk_2 not found please set SDK2_DIR before config")
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${TSDUCK_DIR}/)
    message(FATAL_ERROR "tsduck source not found please set TSDUCK_DIR before config")
endif()

if(MSVC)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(TSDUCK_LIB_DIR	${TSDUCK_DIR}/lib/${TSDUCK_BUILDCONFIG}-Win64)
	else()
		set(TSDUCK_LIB_DIR	${TSDUCK_DIR}/lib/${TSDUCK_BUILDCONFIG}-Win32)
	endif()
	
	if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${TSDUCK_LIB_DIR}/tsduck.lib)
		message(FATAL_ERROR "tsduck must build tsduck dll first ${TSDUCK_LIB_DIR}")
	endif()

endif()

add_subdirectory(${SDK2_DIR} vatek_sdk_2.out)

set(TSDUCK_DIR_INC 
	${TSDUCK_DIR}/include)

set(TSUDCK_VATEK_PLUGIN_SRC
	src/tsduck_vatek_output.cpp)
	
set(TSDUCK_VATEK_PLUGIN_INC
	inc/tsduck_vatek_output.h)
	
set(TSUDCK_VATEK_TOOL_SRC 
	src/tsduck_vatek_main.cpp)

## tsvatek ##

add_executable(${TSDUCK_VATEK_TOOL} ${TSUDCK_VATEK_TOOL_SRC})
add_dependencies(${TSDUCK_VATEK_TOOL} libvatek_core_static)
target_include_directories(${TSDUCK_VATEK_TOOL} PRIVATE
						   ${SDK2_DIR}/api/core/inc
						   ${TSDUCK_DIR_INC})


if(MSVC)
	target_compile_definitions(${TSDUCK_VATEK_TOOL} PRIVATE -D_TSDUCKDLL_USE -D_VA2_STATIC_)
	target_link_directories(${TSDUCK_VATEK_TOOL} PRIVATE ${TSDUCK_LIB_DIR})
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        target_link_directories(${TSDUCK_VATEK_TOOL} PRIVATE "${SDK2_DIR}/build/deps/win/libusb/1.0.24/VS2019/MS64/static")
    else()
		target_link_options(${TSDUCK_VATEK_TOOL} PRIVATE /SAFESEH:NO)
        target_link_directories(${TSDUCK_VATEK_TOOL} PRIVATE "${SDK2_DIR}/build/deps/win/libusb/1.0.24/VS2019/MS32/static")
    endif()
	
	set_target_properties(${TSDUCK_VATEK_TOOL} PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)
    target_compile_options(${TSDUCK_VATEK_TOOL} PRIVATE /wd26495 PRIVATE /wd26812 PRIVATE /wd26498)
    set_property(TARGET ${TSDUCK_VATEK_TOOL} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	
	target_link_libraries(${TSDUCK_VATEK_TOOL} PRIVATE 
						  tsduck.lib
						  libvatek_core_static
						  libusb-1.0.lib)
    source_group("src" FILES ${TSUDCK_VATEK_TOOL_SRC})
endif()

## tsplugin_vatek ##

add_library(${TSDUCK_VATEK_PLUGIN} SHARED ${TSUDCK_VATEK_PLUGIN_SRC} ${TSUDCK_VATEK_PLUGIN_INC})
add_dependencies(${TSDUCK_VATEK_PLUGIN} libvatek_core_static)
target_include_directories(${TSDUCK_VATEK_PLUGIN} PRIVATE 
						   ${SDK2_DIR}/api/core/inc
						   ${TSDUCK_DIR_INC})

if(MSVC)
	target_compile_definitions(${TSDUCK_VATEK_PLUGIN} PRIVATE -D_TSDUCKDLL_USE -D_VA2_STATIC_)
	target_link_directories(${TSDUCK_VATEK_PLUGIN} PRIVATE ${TSDUCK_LIB_DIR})
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        target_link_directories(${TSDUCK_VATEK_PLUGIN} PRIVATE "${SDK2_DIR}/build/deps/win/libusb/1.0.24/VS2019/MS64/static")
    else()
		target_link_options(${TSDUCK_VATEK_PLUGIN} PRIVATE /SAFESEH:NO)
        target_link_directories(${TSDUCK_VATEK_PLUGIN} PRIVATE "${SDK2_DIR}/build/deps/win/libusb/1.0.24/VS2019/MS32/static")
    endif()
	
	set_target_properties(${TSDUCK_VATEK_PLUGIN} PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
    target_compile_options(${TSDUCK_VATEK_PLUGIN} PRIVATE /wd26495 PRIVATE /wd26812 PRIVATE /wd26498)
    set_property(TARGET ${TSDUCK_VATEK_PLUGIN} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	
	target_link_libraries(${TSDUCK_VATEK_PLUGIN} PRIVATE 
						  tsduck.lib
						  libvatek_core_static
						  libusb-1.0.lib)
				
	source_group("inc" FILES ${TSDUCK_VATEK_PLUGIN_INC})
    source_group("src" FILES ${TSUDCK_VATEK_PLUGIN_SRC})
endif()



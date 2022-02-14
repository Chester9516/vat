cmake_minimum_required(VERSION 3.10)

project(app_romtool)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(cli_SRC
    app_romtool.c)

set(cli_SRC_COM
    ../common/src/tool_printf.c)

set(cli_INC_COM
    ../common/inc/tool_printf.h)

if(NOT TARGET libvatek_core)
    message(FATAL_ERROR "app_romtool need build with libvatek_core")
endif()

add_executable(app_romtool ${cli_SRC} ${cli_SRC_COM} ${cli_SRC_INTERNAL})
target_include_directories(app_romtool PRIVATE "inc" "../../api/core/inc")

if(CROSS_ROOT)
	message("cross-compiler used static library")
	target_link_libraries(app_romtool ${CMAKE_BINARY_DIR}/lib/libvatek_core_static.a ${CROSS_ROOT}/lib/libusb-1.0.a pthread)
else()	
	target_link_libraries(app_romtool libvatek_core)
endif()

if(MSVC)
    set_property(TARGET app_romtool PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_target_properties(app_romtool PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)
    target_compile_options(app_romtool PRIVATE /wd26495 PRIVATE /wd26812 PRIVATE /wd26498)

    source_group("common\\inc" FILES ${cli_INC_COM})
    source_group("common\\src" FILES ${cli_SRC_COM})
    source_group("src\\" FILES ${cli_SRC})

elseif(UNIX AND NOT APPLE)
    SET_TARGET_PROPERTIES(app_romtool PROPERTIES INSTALL_RPATH "$ORIGIN/lib:$ORIGIN/")
endif()

set_target_properties(app_romtool PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

cmake_minimum_required(VERSION 3.10)

project(app_broadcast)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(cli_SRC
    app_broadcast.c)

set(cli_SRC_COM
    ../common/src/tool_printf.c
    ../common/src/tool_stream_file.c
    ../common/src/tool_stream_test.c
    ../common/src/tool_stream_udp.c
    ../common/src/tool_tspacket.c)

set(cli_INC_COM
    ../common/inc/tool_printf.h
    ../common/inc/tool_stream.h
    ../common/inc/tool_tspacket.h)

if(NOT TARGET libvatek_core)
    message(FATAL_ERROR "app_broadcast need build with libvatek_core")
endif()

add_executable(app_broadcast ${cli_SRC} ${cli_SRC_COM} ${cli_SRC_INTERNAL})
target_include_directories(app_broadcast PRIVATE "inc" "../../api/core/inc")

if(CROSS_ROOT)
	message("cross-compiler used static library")
	target_link_libraries(app_broadcast ${CMAKE_BINARY_DIR}/lib/libvatek_core_static.a ${CROSS_ROOT}/lib/libusb-1.0.a pthread)
else()	
	target_link_libraries(app_broadcast libvatek_core)
endif()

if(MSVC)
    set_property(TARGET app_broadcast PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_target_properties(app_broadcast PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)
    target_compile_options(app_broadcast PRIVATE /wd26495 PRIVATE /wd26812 PRIVATE /wd26498)

    source_group("common\\inc" FILES ${cli_INC_COM})
    source_group("common\\src" FILES ${cli_SRC_COM})
    source_group("src\\" FILES ${cli_SRC})

elseif(UNIX AND NOT APPLE)
    SET_TARGET_PROPERTIES(app_broadcast PROPERTIES INSTALL_RPATH "$ORIGIN/lib:$ORIGIN/")
endif()

set_target_properties(app_broadcast PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

cmake_minimum_required(VERSION 3.20)
set (CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
else()
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

project(QuasarEngine)

add_compile_definitions($<$<CONFIG:Debug>:QS_DEBUG>)

if(APPLE)
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_C_FLAGS_RELEASE "")

    # LTO is not enabled (LTO often causes MoltenVK/Metal issues)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")

    message("Building on Apple macOS or iOS")
    add_definitions(-DQS_PLATFORM_APPLE)

elseif(WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /GL /Ob2 /Oi /Ot")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /GL /Ob2 /Oi /Ot")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")

    message("Building on Windows")
    add_definitions(-DQS_PLATFORM_WINDOWS)

elseif(UNIX)
    message("Building on Unix-like OS (including Linux)")
    add_definitions(-DQS_PLATFORM_LINUX)

else()
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS_RELEASE "")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(QuasarSoft)
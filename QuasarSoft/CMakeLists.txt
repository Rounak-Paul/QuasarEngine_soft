file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/**/*.cpp
    src/*.cpp
    src/**/*.h
    src/*.h
    src/**/*.inl
    src/main.cpp
)

add_executable(QuasarSoft ${SOURCES})

add_subdirectory(vendor/SDL)

target_include_directories(QuasarSoft PRIVATE 
    src
    vendor/SDL/include
)

target_link_libraries(QuasarSoft PRIVATE 
    SDL3::SDL3-shared
)
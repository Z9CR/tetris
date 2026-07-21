cmake_minimum_required(VERSION 3.14)
#################################################################
# **** ---------------------ATTENTION--------------------- **** #
# ****     program CANT be compiled without this macro     **** #
# ```                                                           #
# #define RAYGUI_IMPLEMENTATION //<-- this line is REQUIRED     #
# include <raygui.h>                                            #
# ```                                                           #
#################################################################

#################################################################
###                                  ||                         #
### please modify following vars    \||/                        #
###                                  \/                         #
#################################################################
project(tetris) # your project name here                        #
set(CMAKE_C_STANDARD 23) # modify it for other vers of C        #
#set(CMAKE_CXX_STANDARD 26) # rm comment to use CPP             #
set(RAYLIB_VERSION 6.0) # modify it for other vers of raylib    #
set(RAYGUI_VERSION 4.0) # modify it for other vers of raygui    #
set(PROJECT_SOURCES_DIRS "src" "include") # your sources here   #
set(PROJECT_INCLUDE_DIRS "include") # your additional libs here #
#################################################################

set(SOURCES "")
foreach(dir ${PROJECT_SOURCES_DIRS})
    file(GLOB_RECURSE dir_sources ${dir}/*.c ${dir}/*.cpp)
    list(APPEND SOURCES ${dir_sources})
endforeach()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

################
# Dependencies #
################
include(FetchContent)

################
# Fetch Raylib #
################
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
    endif()
endif()
################
# Fetch RayGUI #
################
FetchContent_Declare(
        raygui
        URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYGUI_VERSION}.tar.gz
)
FetchContent_GetProperties(raygui)
if (NOT raygui_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raygui)
endif()

###############
# The Project #
###############
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${raygui_SOURCE_DIR}/src)
if(PROJECT_INCLUDE_DIRS)
    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIRS})
endif()

#######################################################################
# Web Configurations                                                  #
#                                                                     #
# Provide a default PLATFORM value to avoid undefined-variable errors #
#######################################################################
if (NOT DEFINED PLATFORM)
    set(PLATFORM "Desktop")
endif()

if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

#############################################
# macOS Frameworks (Only required on macOS) #
#############################################
if (APPLE)
    target_link_libraries(${PROJECT_NAME}
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
endif()
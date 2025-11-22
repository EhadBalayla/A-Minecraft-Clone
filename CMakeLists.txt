cmake_minimum_required(VERSION 3.20)
project(MyMinecraft LANGUAGES C CXX)

# Default to C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable folders in IDEs (Visual Studio, CLion, etc.)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Enable position independent code (needed for shared libs)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#search for the OpenGL API
find_package(OpenGL REQUIRED)
if(NOT OpenGL_FOUND)
	message(FATAL_ERROR "OpenGL not found - either update your drivers or get a new PC you bozo")
endif()





include(FetchContent)

#Add subprojects
add_subdirectory(AMinecraftClone)